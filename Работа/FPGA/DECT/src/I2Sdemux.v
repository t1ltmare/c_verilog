module pcm_to_i2s (
    input lrck,                     // LRCK (Left/Right Clock)
    input sclk,                     // SCLK (Serial Clock)
    input pcm_data,                  // 32-bit PCM data output
    output reg i2s1_data,           // I2S1 Data
    output reg i2s2_data            // I2S2 Data
);

    reg [31:0] pcm_left;
    reg [31:0] pcm_right;
    reg [4:0] bit_cnt;              // Counter to 32, resets by falling enge of LRCK
    reg lrck_d;                     // LRCK delayed 
    reg left_f;                     // Left channel flag

    initial begin
        pcm_left = 0;
        pcm_right = 0;
        bit_cnt = 0;
        lrck_d = 0;
        left_f = 0;
        i2s1_data = 0;
        i2s2_data = 0;
    end

    always @(posedge sclk) begin                                
        if ((!lrck) && lrck_d) begin                            // Bit counter logic, left/right channel detection
            left_f <= 1;          
            bit_cnt <= 0;               
        end else if (lrck && (!lrck_d)) begin    
            left_f <= 0;          
            bit_cnt <= 0;
        end else begin
            bit_cnt <= bit_cnt + 5'b1;
        end
                 
        if (left_f) begin
            pcm_left <= {pcm_left[30:0], pcm_data};
        end else if (!left_f) begin
            pcm_right <= {pcm_right[30:0], pcm_data};
        end
        
        lrck_d <= lrck;
    end

    always @(negedge sclk) begin                        
        if (left_f && (bit_cnt < 16)) begin                     // PCM output
            i2s1_data <= {pcm_right[31 - bit_cnt]};
            i2s2_data <= {pcm_right[31 - (bit_cnt + 16)]};
        end else if ((!left_f) && (bit_cnt < 16)) begin
            i2s1_data <= {pcm_left[31 - bit_cnt]};
            i2s2_data <= {pcm_left[31 - (bit_cnt + 16)]};            
        end else begin
            i2s1_data <= 0;
            i2s2_data <= 0;
        end
    end

endmodule