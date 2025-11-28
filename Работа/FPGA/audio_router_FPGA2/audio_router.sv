module audio_router #(
    parameter REG_COUNT = 96                    // Number of configuration registers
) (
    input   logic         mclk,                 // Основной тактовый сигнал
    input   logic         rst_n,                // Асинхронный сброс 
    input   logic         lrck_192,             // LR clock 48 kHz
    input   logic         sclk_12288,           // Serial clock 3.072 MHz
    input   logic [3:0]   sdi_ff_bus,           // 4 выходные шины 192 kHz
    input   logic [3:0]   sdi_loopback,         // 4 выхода на FPGA2 48 kHz

    output  logic [47:0]  i2s_data_out,         // 48 выходных линий I2S

    // I2C Register interface
    input   logic [7:0]   register_file [0:REG_COUNT-1]  // Configuration registers from I2C
);

// ********** Переменные для создания клоков и детектирования фронтов ********** //

// Переменные тактовых сигналов для 192 кГц шины

logic sclk_3072;
logic lrck_48;

// Счетчики для делителей

logic [0:0] sclk_counter;
logic [0:0] lrck_counter;

// Переменные для детектирования фронтов и спадов lrck

logic lrck_48_sync, lrck_192_sync;
logic lrck_48_rise, lrck_48_fall;
logic lrck_192_rise, lrck_192_fall;

// ********** Делитель частоты для получения sclk_3072, lrck_48 ********** //

// Делитель для sclk_3072 = sclk_12288/4

always_ff @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        sclk_counter <= 1'b0;
        sclk_3072 <= 1'b0;
    end else begin
        sclk_counter <= sclk_counter + 1;
        if (sclk_counter == 1'b1) begin
            sclk_3072 <= ~sclk_3072;
        end
    end
end

// Делитель для lrck_48 = lrck_192/4

always_ff @(negedge lrck_192 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_counter <= 1'b0;
        lrck_48 <= 1'b0;
    end else begin
        lrck_counter <= lrck_counter + 1;
        if (lrck_counter == 1'b1) begin
            lrck_48 <= ~lrck_48;
        end
    end
end

// ******** Синхронизация и детектирование фронтов LRCK ********** //

// Фронты lrck_48

always_ff @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_48_sync <= 1'b0;
    end else begin
        lrck_48_sync <= lrck_48;
    end
end

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_48_rise <= 1'b0;
        lrck_48_fall <= 1'b0;
    end else begin
        lrck_48_rise  <= ~lrck_48_sync && lrck_48;
        lrck_48_fall  <= lrck_48_sync && ~lrck_48;
    end
end

// Фронты lrck_192

always_ff @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_192_sync <= 1'b0;
    end else begin
        lrck_192_sync <= lrck_192;
    end
end

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_192_rise <= 1'b0;
        lrck_192_fall <= 1'b0;
    end else begin      
        lrck_192_rise <= ~lrck_192_sync && lrck_192;
        lrck_192_fall <= lrck_192_sync && ~lrck_192;
    end
end

// ******** Счетчики битов SCLK ********** //

// Переменные счетчиков

logic [4:0] sclk_3072_counter;
logic [4:0] sclk_12288_counter;

// Счетчик SCLK_3072

always_ff @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        sclk_3072_counter <= 5'd0;
    end else begin
        if (lrck_48_rise || lrck_48_fall)
            sclk_3072_counter <= 5'd0;
        else
            sclk_3072_counter <= sclk_3072_counter + 5'd1;
    end
end

// Счетчик SCLK_122288

always_ff @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        sclk_12288_counter <= 5'd0;
    end else begin
        if (lrck_192_rise || lrck_192_fall)
            sclk_12288_counter <= 5'd0;
        else
            sclk_12288_counter <= sclk_12288_counter + 5'd1;
    end
end

// ******** Буферизация входных I2S каналов FF, 32 канала FireFly 192 кГц ******** //

// Переменная для буферизации входов I2S, 32 канала по 24 бита

logic [23:0] ff_channels [0:3][0:7];

// Логика сдвига данных в буфер

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        foreach (ff_channels[i, j]) begin
            ff_channels[i][j] <= 24'b0;
        end
    end else if (sclk_12288_counter < 24) begin
        automatic logic [2:0] base_idx = lrck_48 ? 3'd4 : 3'd0;
        automatic logic [1:0] word_idx; // = sclk_3072_counter[4:3];    // Бит [4:3] дает 0,1,2,3
        
        if (sclk_3072_counter > 5'd30 || sclk_3072_counter < 5'd6) word_idx = 0;
        else if (sclk_3072_counter > 5'd6 && sclk_3072_counter < 5'd14) word_idx = 1;
        else if (sclk_3072_counter > 5'd14 && sclk_3072_counter < 5'd22) word_idx = 2;
        else if (sclk_3072_counter > 5'd22 && sclk_3072_counter < 5'd30) word_idx = 3;

        for (int i = 0; i < 4; i++) begin
            automatic logic [2:0] channel_idx = base_idx + word_idx;
            ff_channels[i][channel_idx] <= {ff_channels[i][channel_idx][22:0], sdi_ff_bus[i]};
        end
    end
end

// ******** Буферизация входных I2S loopback, 8 каналов 48 кГц ******** //

// Переменная для буферизации входов I2S, 8 каналов по 24 бита

logic [23:0] loopback_channels [0:3][0:1];

// Логика сдвига данных в буфер

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        foreach (loopback_channels[i, j]) begin
            loopback_channels[i][j] <= 24'b0;
        end
    end else if (sclk_3072_counter < 24) begin
        automatic logic channel_sel = lrck_48;
        
        for (int i = 0; i < 4; i++) begin
            loopback_channels[i][channel_sel] <= {loopback_channels[i][channel_sel][22:0], sdi_loopback[i]};
        end
    end
end

// ******** Комплектация выходных 96 каналов ******** //

logic [23:0] channel_buffer [0:95]; // 96 отдельных каналов

// Комплектация channel_buffer

always_ff @(negedge sclk_12288_counter[4] or negedge rst_n) begin
    if (!rst_n) begin
        foreach (channel_buffer[i]) begin
            channel_buffer[i] <= 24'b0;
        end
    end else begin
        // Запись только при (!lrck_48 && lrck_48_sync)
        if (!lrck_48 && lrck_48_sync) begin
            for (int channel_idx = 0; channel_idx < 96; channel_idx++) begin
                automatic logic [7:0] reg_val = register_file[channel_idx]; 
                
                if (reg_val < 32) begin
                    // Прямое соответствие между channel_buffer и ff_channels
                    if (channel_idx < 32) begin  // Первые 32 канала из ff_channels
                        automatic logic [1:0] group = channel_idx[4:3];     // group = channel_idx / 8
                        automatic logic [2:0] channel = channel_idx[2:0];   // channel = channel_idx % 8
                        channel_buffer[channel_idx] <= ff_channels[group][channel];
                    end else begin
                        channel_buffer[channel_idx] <= 24'b0;  // За пределами - mute
                    end
                end else if (reg_val < 40) begin
                    // Берем из loopback_channels [0:3][0:1]
                    automatic logic [1:0] group = (reg_val - 32) >> 1;
                    automatic logic channel_sel = (reg_val - 32) & 1'b1;
                    channel_buffer[channel_idx] <= loopback_channels[group][channel_sel];
                end else if (reg_val == 40) begin
                    // Mute - нули
                    channel_buffer[channel_idx] <= 24'b0;
                end else begin
                    // Для остальных значений или по умолчанию - нули
                    channel_buffer[channel_idx] <= 24'b0;
                end
            end
        end
    end
end

// ******** Последовательный вывод выходных шин I2S ******** //

// Регистры для управления последовательным выводом

logic [4:0] bit_index;  // Счетчик битов

always_ff @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 48; bus++) begin
            bit_index[bus] <= '0;
            i2s_data_out[bus] <= 1'b0;
        end
    end else begin
        for (int bus = 0; bus < 48; bus++) begin
            // Обработка битов данных и нулей
            if (bit_index < 24) begin
                if (!lrck_48) i2s_data_out[bus] <= channel_buffer[bus*2][23 - bit_index];
                if (lrck_48) i2s_data_out[bus] <= channel_buffer[bus*2 +1][23 - bit_index];
            end else begin
                i2s_data_out[bus] <= 1'b0;
            end
        end
    end
end

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    // Обновление индексов
    if (!lrck_48 && lrck_48_sync) begin
        bit_index <= '0;
    end else begin
        bit_index <= bit_index + 1;
    end
end

endmodule