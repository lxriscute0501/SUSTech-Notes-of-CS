`timescale 1ns / 1ps

module show_7seg_ego1(
    input [31:0] cnt,
    input clk_out,
    output reg [7:0] a,
    output reg [7:0] b
    );
    parameter s0 = 8'b1111_1100,s1 = 8'b0110_0000,s2 = 8'b1101_1010,s3 = 8'b1111_0010,
        s4 = 8'b0110_0110,s5= 8'b1011_0110,s6 = 8'b1011_1110,s7 = 8'b1110_0000,
        s8 = 8'b1111_1110,s9 = 8'b1111_0110,s10 = 8'b1110_1110,s11 = 8'b0011_1110,
        s12 = 8'b1001_1100,s13 = 8'b0111_1010,s14 = 8'b1001_1110,s15 = 8'b1000_1110;
    always @(posedge clk_out) begin 
    case (cnt%10)
        0: a=s0;
        1: a=s1;
        2: a=s2;
        3: a=s3;
        4: a=s4;
        5: a=s5;
        6: a=s6;
        7: a=s7;
        8: a=s8;
        9: a=s9;
        default: a=s0;
    endcase
    case (cnt/10)
        0: b=s0;
        1: b=s1;
        2: b=s2;
        3: b=s3;
        4: b=s4;
        5: b=s5;
        6: b=s6;
        7: b=s7;
        8: b=s8;
        9: b=s9;
        default: b=s0;
    endcase
end
endmodule
