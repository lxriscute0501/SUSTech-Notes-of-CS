`timescale 1ns / 1ps

module denouncer(
    input old_input,
    input clk,
    input rst_n,
    output reg new_input
    );
    reg[31:0] cnt;
    reg clk_out;
    parameter period = 1000;
    always @(posedge clk, negedge rst_n)//freqency division: clk->clk_out.. 
    begin
        if(~rst_n) begin
            cnt<=0;
            clk_out<=0;
        end
        else begin                     
            if (cnt == (period>>1)-1)
            begin
                clk_out<=~clk_out;
                cnt<=0;
            end   
            else
                cnt<=cnt+1;
        end
    end
    reg [3:0] D;
    always @(posedge clk_out,negedge rst_n) begin
        if (~rst_n) begin
            D<=4'b0000;
        end
        else begin
            D <={old_input,D[3:1]};
        end

    end
    always @(D) begin
        if (D==4'b0000) begin
            new_input<=0;
        end
        else if(D==4'b1111) begin
            new_input<=1;
        end    
    end

endmodule
