`timescale 1ns / 1ps
// this module is used for showing time

module show_time(rst_n,clk,sec_input,min_input,hour_input,seg_en,seg_out0,seg_out1);
input rst_n;//reset: low effect , used to display , not count
input clk;//system clock 100MZ
input [31:0] sec_input;// only in advanced mothed will the input be used
input [31:0] min_input;// only in advanced mothed will the input be used
input [31:0] hour_input;// only in advanced mothed will the input be used
output reg [7:0] seg_en;//selection on tube 0-7 
output [7:0] seg_out0; //seg0 selection
output [7:0] seg_out1; //segl selection
reg clk_out;
reg [31:0] cnt;
reg [3:0] scan_cnt;

parameter period =250000;//500HZ stable
parameter s0 = 8'b0000_0000,s1 = 8'b0000_0001,s2 = 8'b0000_0010,s3 = 8'b0000_0100,
        s4 = 8'b0000_1000,s5 = 8'b0001_0000,s6 = 8'b0010_0000,s7 = 8'b0100_0000,s8 = 8'b1000_0000;

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
always @(posedge clk_out,negedge rst_n) //change scan_cnt based on clkout...
begin
    if(~rst_n)
        scan_cnt<=4'b1000;
    else begin
        if(scan_cnt==4'b0111) 
            scan_cnt<=0;
        else                      
            scan_cnt <= scan_cnt+1;
    end 
end
always @(scan_cnt)//select tube...
begin
    case (scan_cnt)
        4'b0000:seg_en = s1;
        4'b0001:seg_en = s2; 
        4'b0010:seg_en = s3; 
        4'b0011:seg_en = s4; 
        4'b0100:seg_en = s5;    
        4'b0101:seg_en = s6;   
        4'b0110:seg_en = s0; 
        4'b0111:seg_en = s8; 
	    default:seg_en = s0; 
    endcase
end
//module light_7seg_egol(input [3:0]sw,input clk, output reg [7:0]seg_out) 
show_time_light_7seg u0(.sw(scan_cnt),.seg_out(seg_out0),.sec_input(sec_input),.min_input(min_input),.hour_input(hour_input)); 
show_time_light_7seg u1(.sw(scan_cnt),.seg_out(seg_out1),.sec_input(sec_input),.min_input(min_input),.hour_input(hour_input));
endmodule
