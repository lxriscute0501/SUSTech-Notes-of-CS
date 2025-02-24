`timescale 1ns / 1ps

module time_setting(
    input [3:0]scan_cnt,
    input time_setting,// used to set time clk
    input control, // used to select which tube
    input clk, // this is system clk
    inout confirm, 
    input rst_n,
    output reg [7:0]seg_out,
    output reg [31:0] sec_output,
    output reg [31:0] min_output,
    output reg [31:0] hour_output
    );
    //this part used to denounce and test trig (time_setting)
    wire clk_denounce_time;
    wire clk_trig_test_time;// used to keep pace with clk
    denouncer de1(.old_input(time_setting),.new_input(clk_denounce_time),.clk(clk),.rst_n(rst_n));
    trig_test tr1(.clk(clk),.rst_n(rst_n),.trig(clk_denounce_time),.pos(clk_trig_test_time));
    
    //this part used to denounce and test trig (confirm)
    wire clk_denounce_confirm;
    wire clk_trig_test_confirm;// used to keep pace with clk
    denouncer de2(.old_input(confirm),.new_input(clk_denounce_confirm),.clk(clk),.rst_n(rst_n));
    trig_test tr2(.clk(clk),.rst_n(rst_n),.trig(clk_denounce_confirm),.pos(clk_trig_test_confirm));

    //this part used to denounce and test trig (control)
    wire clk_denounce_control;
    wire clk_trig_test_control;// used to keep pace with clk
    denouncer de3(.old_input(control),.new_input(clk_denounce_control),.clk(clk),.rst_n(rst_n));
    trig_test tr3(.clk(clk),.rst_n(rst_n),.trig(clk_denounce_control),.pos(clk_trig_test_control));
    
    
    parameter s0 = 8'b1111_1100,s1 = 8'b0110_0000,s2 = 8'b1101_1010,s3 = 8'b1111_0010,
        s4 = 8'b0110_0110,s5= 8'b1011_0110,s6 = 8'b1011_1110,s7 = 8'b1110_0000,
        s8 = 8'b1111_1110,s9 = 8'b1111_0110,s10 = 8'b1110_1110,s11 = 8'b0011_1110,
        s12 = 8'b1001_1100,s13 = 8'b0111_1010,s14 = 8'b1001_1110,s15 = 8'b1000_1110,
        s16 = 8'b0110_1110;
    
    // this part used to show numbers
    reg [7:0] a,b,c,d,e,f;
    always @ (scan_cnt)
    case(scan_cnt)
        4'b0000: seg_out = a;
        4'b0001: seg_out = b;
        4'b0010: seg_out = c;
        4'b0011: seg_out = d;
        4'b0100: seg_out = e;
        4'b0101: seg_out = f;
        4'b0110: seg_out = s0;
        4'b0111: seg_out = s16;//H
        4'b1000: seg_out = s0;
        default: seg_out =s0;
    endcase
    
    // this part used to control tube and cnt
    reg [7:0] next_a,next_b,next_c,next_d,next_e,next_f;
    reg [31:0] a_cnt,b_cnt,c_cnt,d_cnt,e_cnt,f_cnt;
    reg [5:0] enable;
    reg [5:0] next_enable;
    parameter a1 = 6'b000001,a2 = 6'b000010,a3 = 6'b000100,a4 = 6'b001000,
    a5 = 6'b010000,a6 = 6'b100000;
    
    always @(posedge clk_trig_test_control, negedge rst_n) begin
        if (~rst_n) begin
            enable<=a1;
        end 
        else begin
            enable<=next_enable;
        end
    end
    always @(*) begin
        case (enable)
            a1: next_enable=a2;
            a2: next_enable=a3;
            a3: next_enable=a4;
            a4: next_enable=a5;
            a5: next_enable=a6;
            a6: next_enable=a1;
            default: next_enable=a1;
        endcase
    end
    
    always @(posedge clk_trig_test_time , negedge rst_n) begin
        if (~rst_n) begin
            a_cnt<=0;
            a<=s0;
            b_cnt<=0;
            b<=s0;
            c_cnt<=0;
            c<=s0;
            d_cnt<=0;
            d<=s0;
            e_cnt<=0;
            e<=s0;
            f_cnt<=0;
            f<=s0;
        end else begin
            if (enable[0]) begin
                a<=next_a;
                if (a_cnt==9) begin
                    a_cnt<=0;
                end
                else begin
                    a_cnt <= a_cnt +1;
                end
            end else if(enable[1]) begin
                b<=next_b;
                if (b_cnt==5) begin
                    b_cnt<=0;
                end
                else begin
                    b_cnt <= b_cnt +1;
                end
            end else if(enable[2]) begin
                c<=next_c;
                if (c_cnt==9) begin
                    c_cnt<=0;
                end
                else begin
                    c_cnt <= c_cnt +1;
                end
            end else if(enable[3]) begin
                d<=next_d;
                if (d_cnt==5) begin
                    d_cnt<=0;
                end
                else begin
                    d_cnt <= d_cnt +1;
                end
            end else if(enable[4]) begin
                e<=next_e;
                if (e_cnt==9) begin
                    e_cnt<=0;
                end
                else begin
                    e_cnt <= e_cnt +1;
                end
            end else if(enable[5]) begin
                f<=next_f;
                if (f_cnt==2) begin
                    f_cnt<=0;
                end
                else begin
                    f_cnt <= f_cnt +1;
                end
            end
            
        end
    end
    always @(a) begin
        case (a)
            s0: next_a=s1; 
            s1: next_a=s2; 
            s2: next_a=s3; 
            s3: next_a=s4; 
            s4: next_a=s5; 
            s5: next_a=s6; 
            s6: next_a=s7; 
            s7: next_a=s8; 
            s8: next_a=s9; 
            s9: next_a=s0;
            default: next_a<=s0;
        endcase
    end
    always @(b) begin
        case (b)
            s0: next_b=s1; 
            s1: next_b=s2; 
            s2: next_b=s3; 
            s3: next_b=s4; 
            s4: next_b=s5; 
            s5: next_b=s0; 
            default: next_b=s0;
        endcase
    end
    always @(c) begin
        case (c)
            s0: next_c=s1; 
            s1: next_c=s2; 
            s2: next_c=s3; 
            s3: next_c=s4; 
            s4: next_c=s5; 
            s5: next_c=s6; 
            s6: next_c=s7; 
            s7: next_c=s8; 
            s8: next_c=s9; 
            s9: next_c=s0;
            default: next_c=s0;
        endcase
    end
    always @(d) begin
        case (d)
            s0: next_d=s1; 
            s1: next_d=s2; 
            s2: next_d=s3; 
            s3: next_d=s4; 
            s4: next_d=s5; 
            s5: next_d=s0; 
            default: next_d=s0;
        endcase
    end
    always @(e) begin
        case (e)
            s0: next_e=s1; 
            s1: next_e=s2; 
            s2: next_e=s3; 
            s3: next_e=s4; 
            s4: next_e=s5; 
            s5: next_e=s6; 
            s6: next_e=s7; 
            s7: next_e=s8; 
            s8: next_e=s9; 
            s9: next_e=s0;
            default: next_e=s0;
        endcase
    end
    always @(f) begin
        case (f)
            s0: next_f=s1; 
            s1: next_f=s2; 
            s2: next_f=s0; 
        endcase
    end

    always @(posedge clk_trig_test_confirm , negedge rst_n) begin
        if (~rst_n) begin
            min_output<=0;
            sec_output<=0;
            hour_output<=0;
        end else begin
            sec_output<=a_cnt+b_cnt*10;
            min_output<=c_cnt+d_cnt*10;
            if (e_cnt+f_cnt*10>23) begin
                hour_output<=23;
            end
            else
                hour_output<=e_cnt+f_cnt*10; 
            end    
    end

endmodule
