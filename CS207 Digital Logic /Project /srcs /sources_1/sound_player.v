`timescale 1ns / 1ps

module sound_player(
    input clk,
    input rst_n,
    output reg sound
    );
    reg clk_out;//4Hz
    reg [31:0] cnt1 ;
    parameter d1 = 25000000;
    always @(posedge clk,negedge rst_n) begin
        if (~rst_n) begin
            clk_out<=0;
            cnt1<=0;
        end
        else begin
            if (cnt1==(d1>>1)-1) begin
                clk_out<=~clk_out;
                cnt1<=0;
            end
            else
                cnt1<=cnt1+1;
        end
    end
    reg [4:0] count;
    reg [31:0] orign;
    always @(posedge clk_out,negedge rst_n) begin
        if (~rst_n) begin
            count<=0;
        end
        else begin
            count<=count+1;
            if (count==17) begin
                count<=1;
            end
            case (count)
                1: orign<=0;
                2: orign<=0;
                3: orign<=13915;
                4: orign<=13915;
                5: orign<=31766;
                6: orign<=31766;
                7: orign<=42517;
                8: orign<=42517;
                9: orign<=63776;
                10: orign<=63776;
                11: orign<=63776;
                12: orign<=51794;
                13: orign<=51794;
                14: orign<=51794;
                15: orign<=31766;
                16: orign<=31766;
                17: orign<=31766;
                default: orign<=0; 
            endcase
        end
    end
    reg [31:0] cnt2;
    always @(posedge clk,negedge rst_n) begin
        if (~rst_n) begin
            sound<=0;
            cnt2<=orign;
        end
        else begin
            if (cnt2==127551) begin
                cnt2<=orign;
                sound<=~sound;
            end
            else begin
                cnt2<=cnt2+1;
            end
        end
    end
    
endmodule
