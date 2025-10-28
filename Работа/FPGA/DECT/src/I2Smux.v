module i2s_to_pcm (
    input lrck,                     // LRCK (Left/Right Clock)
    input sclk,                     // SCLK (Serial Clock)
    input i2s1_data,                // I2S1 Data
    input i2s2_data,                // I2S2 Data
    output reg pcm_data             // 32-bit PCM data output
);

    reg [15:0] i2s1_left;
    reg [15:0] i2s2_left;
    reg [15:0] i2s1_right;
    reg [15:0] i2s2_right;
    reg [4:0] bit_cnt;              // Counter to 32, resets by falling enge of LRCK
    reg lrck_d;                     // LRCK delayed 
    reg left_f;                     // Left channel flag

    initial begin
        i2s1_left <= 0;
        i2s2_left <= 0;
        i2s1_right <= 0;
        i2s2_right <= 0;
        bit_cnt <= 0;
        lrck_d <= 0;
        left_f <= 0;
        pcm_data <= 0;
    end

    always @(posedge sclk) begin                                
        if ((!lrck) && lrck_d) begin                            // Bit counter logic, left/right channel detection
            left_f <= 1;          
            bit_cnt <= 0;               
        end else if (lrck && (!lrck_d)) begin    
            left_f <= 0;          
            bit_cnt <= 0;
        end else begin
            bit_cnt <= bit_cnt + 5'b00001;
        end

        lrck_d <= lrck;                                         // LRCK delay
 
        if (bit_cnt < 16 && lrck == lrck_d) begin               // I2S write to registers
            if (left_f) begin
                i2s1_left <= {i2s1_left[14:0], i2s1_data};
                i2s2_left <= {i2s2_left[14:0], i2s2_data};
            end else if (!left_f) begin
                i2s1_right <= {i2s1_right[14:0], i2s1_data};
                i2s2_right <= {i2s2_right[14:0], i2s2_data};
            end
        end
    end

    always @(negedge sclk) begin                        
        if (left_f && (bit_cnt < 16)) begin                     // PCM output
            pcm_data <= {i2s1_right[15 - bit_cnt]};   
        end else if (left_f && (bit_cnt < 32)) begin
            pcm_data <= {i2s2_right[15 - (bit_cnt - 16)]};
        end else if ((!left_f) && (bit_cnt < 16)) begin
            pcm_data <= {i2s1_left[15 - bit_cnt]};
        end else if ((!left_f) && (bit_cnt < 32)) begin
            pcm_data <= {i2s2_left[15 - (bit_cnt - 16)]};
        end else begin
            pcm_data <= 0;
        end
    end

endmodule