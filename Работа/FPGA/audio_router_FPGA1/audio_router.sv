module audio_router #(
    parameter REG_COUNT = 40                // Number of configuration registers
) (
    input  logic         mclk,              // Основной тактовый сигнал
    input  logic         rst_n,             // Асинхронный сброс 
    input  logic         lrck_192,           // LR clock 48 kHz
    input  logic         sclk_12288,         // Serial clock 3.072 MHz
    input  logic [47:0]  i2s_data_in,       // 48 входных линий I2S
    output logic [3:0]   sdo_ff_bus,        // 4 выходные шины 192 kHz
    output logic [3:0]   sdo_loopback,      // 4 выхода на FPGA2 48 kHz

    // I2C Register interface
    input  logic [7:0]   register_file [0:REG_COUNT-1]  // Configuration registers from I2C
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

// ******** Буферизация входных I2S каналов, 48 входов -> 96 каналов ******** //

// Переменная для буферизации входов I2S, 96 каналов по 24 бита

logic [23:0] channel_buffer [0:95];

// Буферизация левых (!lrck_48) в четные channel_buffer (0, 2, 4 ...)

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 96; i++) begin
            channel_buffer[i] <= 24'b0;
        end
    end else begin
        if (!lrck_48_sync && sclk_3072_counter < 24) begin
            for (int i = 0; i < 48; i++) begin
                channel_buffer[2*i] <= {channel_buffer[2*i][22:0], i2s_data_in[i]}; // 0L, 1L, 2L...
            end
        end else begin
            for (int i = 0; i < 48; i++) begin
                channel_buffer[2*i] = channel_buffer[2*i];
            end
        end
    end
end

// Буферизация правых (lrck_48) в нечетные channel_buffer (1, 3, 5 ...) 

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        // Буферы уже сброшены в предыдущем блоке
    end else begin
        if (lrck_48_sync && sclk_3072_counter < 24) begin
            for (int i = 0; i < 48; i++) begin
                channel_buffer[2*i + 1] <= {channel_buffer[2*i + 1][22:0], i2s_data_in[i]}; // 0R, 1R, 2R...
            end 
        end else begin
            for (int i = 0; i < 48; i++) begin
                channel_buffer[2*i + 1] = channel_buffer[2*i + 1];
            end
        end
    end
end

// ******** Комплектация в выходные шины ff_channels, 4 шины по 8 каналов, 192 кГц ******** //

// Формирование выходных шин sdo_ff_bus

logic [23:0] ff_channels [0:3][0:7];  

// Инициализация ff_channels при сбросе

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            for (int j = 0; j < 8; j++) begin
                ff_channels[i][j] <= 24'b0;
            end
        end
    end else begin
        if (!lrck_192 && lrck_192_sync && !lrck_48 && lrck_48_sync) begin
            for (int i = 0; i < 8; i++) begin
                ff_channels[0][i] <= channel_buffer[register_file[i]];
            end
            for (int i = 0; i < 8; i++) begin
                ff_channels[1][i] <= channel_buffer[register_file[8 + i]];
            end
            for (int i = 0; i < 8; i++) begin
                ff_channels[2][i] <= channel_buffer[register_file[16 + i]];
            end
            for (int i = 0; i < 8; i++) begin
                ff_channels[3][i] <= channel_buffer[register_file[24 + i]];
            end
        end
    end
end

// ******** Последовательный вывод выходных шин ff_channels ******** //

// Последовательная передача для sdo_ff_bus с форматом 24 бита + 8 нулей

logic [2:0] ff_channel_index [0:3];  // Индекс текущего канала (0-7)
logic [4:0] ff_bit_index [0:3];      // Индекс бита (0-31: 24 данных + 8 нулей)

always_ff @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            ff_channel_index[i] <= '0;
            ff_bit_index[i] <= '0;
        end
        sdo_ff_bus <= '0;
    end else begin
        for (int bus = 0; bus < 4; bus++) begin
            // Обработка битов данных и нулей
            if (ff_bit_index[bus] < 24) begin       // Вывод битов данных (MSB first)                
                sdo_ff_bus[bus] <= ff_channels[bus][ff_channel_index[bus]][23 - ff_bit_index[bus]];
            end else begin
                sdo_ff_bus[bus] <= 1'b0;            // Вывод нулей после канала
            end

            // Обновление индексов
            if (ff_bit_index[bus] == 31) begin
                ff_bit_index[bus] <= '0;
                if (ff_channel_index[bus] == 7) begin
                    ff_channel_index[bus] <= '0;
                end else begin
                    ff_channel_index[bus] <= ff_channel_index[bus] + 1;
                end
            end else begin
                ff_bit_index[bus] <= ff_bit_index[bus] + 1;
            end
        end
    end
end

// ******** Комплектация в выходные шины loopback_channels, 4 шины по 2 канала, 48 кГц ******** //

logic [23:0] loopback_channels [0:3][0:1]; // 4 шины по 2 канала
logic [0:0] loopback_channel_index [0:3];  // индекс канала (0 или 1)
logic [4:0] loopback_bit_index [0:3];      // битовый индекс (0-31)

// Формирование выходных шин loopback_channels 

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            for (int j = 0; j < 2; j++) begin
                loopback_channels[i][j] <= 24'b0;
            end
        end
    end else begin
        if (!lrck_48 && lrck_48_sync) begin
            for (int i = 0; i < 2; i++) begin
                loopback_channels[0][i] <= channel_buffer[register_file[32 + i]];
            end
            for (int i = 0; i < 2; i++) begin
                loopback_channels[1][i] <= channel_buffer[register_file[34 + i]];
            end
             for (int i = 0; i < 2; i++) begin
                loopback_channels[2][i] <= channel_buffer[register_file[36 + i]];
            end
            for (int i = 0; i < 2; i++) begin
                loopback_channels[3][i] <= channel_buffer[register_file[38 + i]];
            end
        end
    end
end


// ******** Последовательный вывод выходных шин loopback_channels ******** //

always_ff @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            loopback_channel_index[i] <= '0;
            loopback_bit_index[i] <= '0;
            sdo_loopback <= '0;
        end
        sdo_loopback <= '0;
    end else begin
        for (int bus = 0; bus < 4; bus++) begin
            // Обработка битов данных и нулей
            if (loopback_bit_index[bus] < 24) begin     // Вывод битов данных (MSB first)                
                sdo_loopback[bus] <= loopback_channels[bus][loopback_channel_index[bus]][23 - loopback_bit_index[bus]];
            end else begin
                sdo_loopback[bus] <= 1'b0;              // Вывод нулей после канала
            end

            // Обновление индексов
            if (loopback_bit_index[bus] == 31) begin
                loopback_bit_index[bus] <= '0;
                loopback_channel_index[bus] <= ~loopback_channel_index[bus];
            end else begin
                loopback_bit_index[bus] <= loopback_bit_index[bus] + 1;
            end
        end
    end
end

endmodule