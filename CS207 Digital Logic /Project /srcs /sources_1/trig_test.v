`timescale 1ns / 1ps


module trig_test(input clk,rst_n,trig, output pos);
    reg trig1,trig2,trig3;
    always @(posedge clk, negedge rst_n)
    if(~rst_n)
        {trig1,trig2,trig3} <= 3'b000;
    else begin
        trig1 <= trig;
        trig2 <= trig1;
        trig3 <= trig2;
    end
    assign pos = (~trig3) & trig2;
 
endmodule
