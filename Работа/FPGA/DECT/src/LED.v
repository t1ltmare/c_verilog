module rgb_blink (
  input  wire clk,       // Тактовый сигнал
  output reg red,        // Красный светодиод (активный низкий)
  output reg green,      // Зеленый светодиод (активный низкий)
  output reg blue        // Синий светодиод (активный низкий)
);

    parameter [24:0] BLINK_PERIOD = 25'd24576000 / 2;

    reg [25:0] counter;  // Счетчик для определения интервала моргания
    reg [1:0]  color_state; // Состояние цвета: 00 - Red, 01 - Green, 10 - Blue, 11 - Red Blue, 100 - Red Green, 101 - Blue Green, 110 - Red Green Blue

    initial begin
        color_state = 0;
        counter = 0; 
    end

    always @(posedge clk) begin
        counter <= counter + 1'b1;

        if (counter == BLINK_PERIOD - 25'b1) begin
          counter <= 25'b0;
          color_state <= color_state + 2'b01;
          if (color_state == 2'b11) begin
            color_state <= 2'b00;  // Вернуться к красному после синего
          end
        end

        case (color_state)
            2'b00: begin // Red
                red   <= 1'b0;  // Красный включен (0)
                green <= 1'b1;  // Зеленый выключен (1)
                blue  <= 1'b1;  // Синий выключен (1)
            end
            2'b01: begin // Green
                red   <= 1'b1;  // Красный выключен (1)
                green <= 1'b0;  // Зеленый включен (0)
                blue  <= 1'b1;  // Синий выключен (1)
            end
            2'b10: begin // Blue
                red   <= 1'b1;  // Красный выключен (1)
                green <= 1'b1;  // Зеленый выключен (1)
                blue  <= 1'b0;  // Синий включен (0)
            end
            2'b11: begin // Green Blue
                red   <= 1'b1;  // Красный выключен (1)
                green <= 1'b0;  // Зеленый включен (0)
                blue  <= 1'b0;  // Синий включен (0)
            end
            default: begin // 
                red   <= 1'b1;  // Красный выключен (1)
                green <= 1'b1;  // Зеленый выключен (1)
                blue  <= 1'b1;  // Синий выключен (1)
            end
        endcase
    end

endmodule