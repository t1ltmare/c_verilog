module audio_router #(
    parameter REG_COUNT = 40                // Number of configuration registers
) (
    input  logic         mclk,              // Основной тактовый сигнал
    input  logic         rst_n,             // Асинхронный сброс 
    input  logic         lrck_48,           // LR clock 48 kHz
    input  logic         sclk_3072,         // Serial clock 3.072 MHz
    input  logic [47:0]  i2s_data_in,       // 48 входных линий I2S
    output logic [3:0]   sdo_ff_bus,        // 4 выходные шины 192 kHz
    output logic [3:0]   sdo_loopback,      // 4 выхода на FPGA2 48 kHz

    // I2C Register interface
    input  logic [7:0]   register_file [0:REG_COUNT-1]  // Configuration registers from I2C
);


// Переменные тактовых сигналов для 192 кГц шины
logic sclk_12288;
logic lrck_192;

// Переменные для детектирования фронтов и спадов lrck
logic lrck_48_sync, lrck_192_sync;
logic lrck_48_prev, lrck_192_prev;
logic lrck_48_rise, lrck_48_fall;
logic lrck_192_rise, lrck_192_fall;

// Делитель частоты для SCLK 12.288 МГц
always_ff @(posedge mclk or negedge rst_n) begin
    if (!rst_n) begin
        sclk_12288 <= 1'b0;
    end else begin
        sclk_12288 <= ~sclk_12288;
    end
end

// Делитель частоты для LRCK 192 КГц
logic [6:0] lrck_counter;
always_ff @(posedge mclk or negedge rst_n) begin
    if (!rst_n) begin
        lrck_counter <= 7'd0;
        lrck_192 <= 1'b0;
    end else begin
        if (lrck_counter == 7'd127) begin
            lrck_counter <= 7'd0;
            lrck_192 <= ~lrck_192;
        end else begin
            lrck_counter <= lrck_counter + 7'd1;
        end
    end
end

// Синхронизация и детектирование фронтов
always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_48_sync <= 1'b0;
        lrck_48_prev <= 1'b0;
        lrck_48_rise <= 1'b0;
        lrck_48_fall <= 1'b0;
    end else begin
        lrck_48_sync <= lrck_48;
        lrck_48_prev <= lrck_48_sync;
        
        lrck_48_rise  <= ~lrck_48_prev && lrck_48_sync;
        lrck_48_fall  <= lrck_48_prev && ~lrck_48_sync;
    end
end

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_192_sync <= 1'b0;
        lrck_192_prev <= 1'b0;
        lrck_192_rise <= 1'b0;
        lrck_192_fall <= 1'b0;
    end else begin
        lrck_192_sync <= lrck_192;
        lrck_192_prev <= lrck_192_sync;
        
        lrck_192_rise <= ~lrck_192_prev && lrck_192_sync;
        lrck_192_fall <= lrck_192_prev && ~lrck_192_sync;
    end
end

// Счетчики битов
logic [4:0] sclk_3072_counter;
logic [4:0] sclk_12288_counter;

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        sclk_3072_counter <= 5'd0;
    end else begin
        if (lrck_48_rise || lrck_48_fall)
            sclk_3072_counter <= 5'd0;
        else
            sclk_3072_counter <= sclk_3072_counter + 5'd1;
    end
end

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        sclk_12288_counter <= 5'd0;
    end else begin
        if (lrck_192_rise || lrck_192_fall)
            sclk_12288_counter <= 5'd0;
        else
            sclk_12288_counter <= sclk_12288_counter + 5'd1;
    end
end

// Буферизация 96 каналов
logic [23:0] channel_buffer [0:95]; // 96 каналов по 24 бита

// Запись левых (!lrck_48) (нечетные/левые каналы - 0, 2, 4, 6 ...)
always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 96; i++) begin
            channel_buffer[i] <= 24'b0;
        end
    end else begin
        if (!lrck_48_sync && !lrck_48_rise && !lrck_48_fall && sclk_3072_counter < 24) begin
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

// Запись правых (lrck_48) (четные/правые каналы - 1, 3, 5, 7 ...)  
always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        // Буферы уже сброшены в предыдущем блоке
    end else begin
        if (lrck_48_sync && !lrck_48_rise && !lrck_48_fall && sclk_3072_counter < 24) begin
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

// Формирование выходных шин sdo_ff_bus
logic [23:0] ff_channels [0:3][0:7];    // 4 шины по 8 каналов

// Инициализация ff_channels при сбросе
always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        // Инициализируем все каналы нулями
        for (int i = 0; i < 4; i++) begin
            for (int j = 0; j < 8; j++) begin
                ff_channels[i][j] <= 24'b0;
            end
        end
    end else begin
        // Обновляем данные из channel_buffer согласно register_file
        if (lrck_48_fall) begin
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

// Последовательная передача для sdo_ff_bus с форматом 24 бита + 8 нулей
logic [2:0] ff_channel_index [0:3];  // Индекс текущего канала (0-7)
logic [4:0] ff_bit_index [0:3];      // Индекс бита (0-31: 24 данных + 8 нулей)

generate
for (genvar i = 0; i < 4; i++) begin : ff_bus_output
    always_ff @(posedge sclk_12288 or negedge rst_n) begin
        if (!rst_n) begin
            ff_channel_index[i] <= 3'd0;
            ff_bit_index[i] <= 5'd31;
            sdo_ff_bus[i] <= 1'b0;
        end else begin
            if (lrck_192_rise) begin
                // Начало кадра - сброс индексов
                ff_channel_index[i] <= 3'd0;
                ff_bit_index[i] <= 5'd31;  // Начинаем с бита 31 (MSB данных)
                
                // Передаем MSB первого канала (бит 23 данных)
                sdo_ff_bus[i] <= ff_channels[i][0][23];
                
            end else if (lrck_192_fall) begin
                // Пауза между кадрами
                sdo_ff_bus[i] <= 1'b0;
                
            end else begin
                if (ff_bit_index[i] == 5'd0) begin
                    // Переход к следующему каналу
                    ff_channel_index[i] <= ff_channel_index[i] + 3'd1;
                    ff_bit_index[i] <= 5'd31;  // Сбрасываем битовый индекс
                    
                    if (ff_channel_index[i] < 3'd7) begin
                        // Передаем MSB следующего канала (бит 23 данных)
                        sdo_ff_bus[i] <= ff_channels[i][ff_channel_index[i] + 3'd1][23];
                    end else begin
                        // Все каналы переданы, выводим 0
                        sdo_ff_bus[i] <= 1'b0;
                    end
                    
                end else begin
                    // Следующий бит
                    ff_bit_index[i] <= ff_bit_index[i] - 5'd1;
                    
                    // Определяем, передаем ли мы данные или нули
                    if (ff_bit_index[i] > 5'd7) begin
                        // Передаем биты данных (23..0)
                        sdo_ff_bus[i] <= ff_channels[i][ff_channel_index[i]][ff_bit_index[i] - 5'd8];
                    end else begin
                        // Передаем нули (биты 7..0)
                        sdo_ff_bus[i] <= 1'b0;
                    end
                end
            end
        end
    end
end
endgenerate

// LoopBack выходы
logic [23:0] loopback_channels [0:3][0:1]; // 4 шины по 2 канала

always_comb begin
    // sdo_loopback[0] - каналы 32 и 33
    loopback_channels[0][0] = channel_buffer[register_file[32]];
    loopback_channels[0][1] = channel_buffer[register_file[33]];
    
    // sdo_loopback[1] - каналы 34 и 35
    loopback_channels[1][0] = channel_buffer[register_file[34]];
    loopback_channels[1][1] = channel_buffer[register_file[35]];
    
    // sdo_loopback[2] - каналы 36 и 37
    loopback_channels[2][0] = channel_buffer[register_file[36]];
    loopback_channels[2][1] = channel_buffer[register_file[37]];
    
    // sdo_loopback[3] - каналы 38 и 39
    loopback_channels[3][0] = channel_buffer[register_file[38]];
    loopback_channels[3][1] = channel_buffer[register_file[39]];
end

// Последовательная передача для sdo_loopback 
logic loopback_channel_sel [0:3];
logic [4:0] loopback_bit_index [0:3];

generate
for (genvar i = 0; i < 4; i++) begin : loopback_output
    always_ff @(posedge sclk_3072 or negedge rst_n) begin
        if (!rst_n) begin
            loopback_channel_sel[i] <= 1'b0;
            loopback_bit_index[i] <= 5'd23;
            sdo_loopback[i] <= 1'b0;
        end else begin
            if (lrck_48_rise) begin
                loopback_channel_sel[i] <= 1'b0;
                loopback_bit_index[i] <= 5'd23;
                sdo_loopback[i] <= loopback_channels[i][0][23];
            end else if (lrck_48_fall) begin
                loopback_channel_sel[i] <= 1'b1;
                loopback_bit_index[i] <= 5'd23;
                sdo_loopback[i] <= loopback_channels[i][1][23];
            end else begin
                if (loopback_bit_index[i] == 5'd0) begin
                    // 8 бит нулей после 24 бит данных
                    sdo_loopback[i] <= 1'b0;
                    loopback_bit_index[i] <= loopback_bit_index[i] - 5'd1;
                end else if (loopback_bit_index[i] > 5'd0) begin
                    // Следующий бит данных
                    loopback_bit_index[i] <= loopback_bit_index[i] - 5'd1;
                    sdo_loopback[i] <= loopback_channels[i][loopback_channel_sel[i]][loopback_bit_index[i] - 5'd1];
                end else begin
                    // Оставшиеся нули
                    sdo_loopback[i] <= 1'b0;
                end
            end
        end
    end
end
endgenerate

endmodule