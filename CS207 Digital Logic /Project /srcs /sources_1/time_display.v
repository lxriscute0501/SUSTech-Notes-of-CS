`timescale 1ns / 1ps
// this module is only used for test 

module time_display(
    input rst_n,//reset: low effect
    input clk,//system clock 100MZ
    input stop,//used to count or not
    output [7:0] seg_en,//selection on tube 0-7 
    output [7:0] seg_out0, //seg0 selection
    output [7:0] seg_out1 //segl selection
    );
  
endmodule

