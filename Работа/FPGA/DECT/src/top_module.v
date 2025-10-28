module top_module (
    input clk,              // System clock
    output sclk,            // Serial clock
    output lrck,            // Left/Right clock (изменено на lrck)
    output i2s1_data,       // I2S Data 1
    output i2s2_data,       // I2S Data 2
    output pcm_data,        // Serial PCM data output
    output i2s1_data_out,   // Demuxed I2S Data 1
    output i2s2_data_out,   // Demuxed I2S Data 2
    output red,         // Красный светодиод (активный низкий)
    output green,       // Зеленый светодиод (активный низкий)
    output blue         // Синий светодиод (активный низкий)
);

    wire  sclk_internal;
    wire  lrck_internal;  
    wire  i2s1_data_internal;
    wire  i2s2_data_internal;

    i2s_generator i2s_gen (
        .clk(clk),
        .sclk(sclk_internal),
        .lrck(lrck_internal), 
        .i2s1_data(i2s1_data_internal),
        .i2s2_data(i2s2_data_internal)
    );

    i2s_to_pcm i2s_mux (
        .lrck(lrck_internal), 
        .sclk(sclk_internal),
        .i2s1_data(i2s1_data_internal),
        .i2s2_data(i2s2_data_internal),
        .pcm_data(pcm_data)
    );

    pcm_to_i2s i2s_demux (
        .lrck(lrck_internal),                   
        .sclk(sclk_internal),                   
        .pcm_data(pcm_data),                    
        .i2s1_data(i2s1_data_out),              
        .i2s2_data(i2s2_data_out)               
);

    rgb_blink rgb_blink (
        .clk(clk),       // Тактовый сигнал
        .red(red),        // Красный светодиод (активный низкий)
        .green(green),      // Зеленый светодиод (активный низкий)
        .blue(blue)        // Синий светодиод (активный низкий)
);

    assign sclk      = sclk_internal;
    assign lrck      = lrck_internal; 
    assign i2s1_data  = i2s1_data_internal;
    assign i2s2_data  = i2s2_data_internal;

endmodule