`timescale 1ns / 1ps

module working_time_light_7seg(
    input [3:0] sw,
    input clk,
    input rst_n,
    input [31:0] sec_input,
    input [31:0] min_input,
    input [31:0] hour_input,
    input stop,
    output reg [7:0]seg_out,
    output reg time_out
);
wire [7:0] a,b,c,d,e,f,g,h;
reg [31:0] cnt;
reg [31:0] sec;
reg [31:0] min;
reg [31:0] hour;
reg clk_out;
parameter period = 100000000;
parameter s0 = 8'b1111_1100,s1 = 8'b0110_0000,s2 = 8'b1101_1010,s3 = 8'b1111_0010,
        s4 = 8'b0110_0110,s5= 8'b1011_0110,s6 = 8'b1011_1110,s7 = 8'b1110_0000,
        s8 = 8'b1111_1110,s9 = 8'b1111_0110,s10 = 8'b1110_1110,s11 = 8'b0011_1110,
        s12 = 8'b1001_1100,s13 = 8'b0111_1010,s14 = 8'b1001_1110,s15 = 8'b1000_1110;
always @(posedge clk)//freqency division: clk->clk_out.. 
begin
    begin                     
        if (cnt == (period>>1)-1)
        begin
            clk_out<=~clk_out;
            cnt<=0;
        end   
        else
            cnt<=cnt+1;
    end
end
always @(posedge clk_out,negedge rst_n) begin // set sec,min,hour counter
    if (~rst_n) begin
        sec<=0;
        min<=0;
        hour<=0;
        time_out<=0;
    end
    else
    begin
        if (~stop) begin
            if (sec == 59) begin
                sec <= 0; 
                if (min == 59) begin
                    min <= 0; 
                    if (hour == 23) begin
                        hour <= 0; 
                    end 
                    else begin
                        hour <= hour + 1;
                    end
                end 
                else begin
                    min <= min + 1;
                end
            end 
            else begin
                sec <= sec + 1;
            end
        end
        
        if (hour==hour_input && min==min_input && sec==sec_input) begin
            time_out<=1;
        end
    end    
end

show_7seg_ego1 sh1(.clk_out(clk_out),.cnt(sec),.a(a),.b(b));//sec display
show_7seg_ego1 sh2(.clk_out(clk_out),.cnt(min),.a(c),.b(d));//min display
show_7seg_ego1 sh3(.clk_out(clk_out),.cnt(hour),.a(e),.b(f));//hour display

wire [7:0] twenkle;
twenkle tw(.clk(clk),.rst_n(rst_n),.enable(twenkle));
always @ (*)begin
    if (time_out) begin
        case(sw)
            4'b0000: seg_out = a & twenkle;
            4'b0001: seg_out = b & twenkle;
            4'b0010: seg_out = c & twenkle;
            4'b0011: seg_out = d & twenkle;
            4'b0100: seg_out = e & twenkle;
            4'b0101: seg_out = f & twenkle;
            4'b0110: seg_out = g;
            4'b0111: seg_out = s15;//F
            4'b1000: seg_out = s0;
            default: seg_out =s0;
        endcase
    end
    else begin
        case(sw)
            4'b0000: seg_out = a;
            4'b0001: seg_out = b;
            4'b0010: seg_out = c;
            4'b0011: seg_out = d;
            4'b0100: seg_out = e;
            4'b0101: seg_out = f;
            4'b0110: seg_out = g;
            4'b0111: seg_out = s15;//F
            4'b1000: seg_out = s0;
            default: seg_out =s0;
        endcase
    end    
end
endmodule
