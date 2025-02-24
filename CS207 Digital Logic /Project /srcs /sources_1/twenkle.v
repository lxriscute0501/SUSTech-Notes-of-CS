`timescale 1ns / 1ps

module twenkle(
    input clk,
    input rst_n,
    output reg [7:0] enable
    );
    parameter period = 10000000;
    reg [31:0] cnt;
    always @(posedge clk,negedge rst_n)//freqency division: clk->clk_out.. 
    begin
        if (~rst_n) begin
            enable <=8'b0000_0000;
            cnt<=0;
        end
        else 
        begin                     
            if (cnt == (period>>1)-1)
            begin
                enable<=~enable;
                cnt<=0;
            end   
            else
                cnt<=cnt+1;
        end
    end
endmodule
