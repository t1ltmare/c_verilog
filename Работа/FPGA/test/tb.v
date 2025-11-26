`timescale 1 ps/ 1 ps
module counter_vlg_tst();

// test vector input registers
reg clk;
reg enable;
reg reset;
// wires                                               
wire [3:0]  count;

// assign statements (if any)                          
counter i1 (
// port map - connection between master ports and signals/registers   
        .clk(clk),
        .count(count),
        .enable(enable),
        .reset(reset)
);

initial                                                
begin                                                  
  clk = 0;
  enable = 0;
  reset = 0;                                           
  $display("Running testbench");                       
end                                                    

always 
  #5  clk =  ! clk;    //создание clk                                                
 
endmodule