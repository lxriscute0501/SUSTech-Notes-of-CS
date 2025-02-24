`timescale 1ns / 1ps

module time_setting_controller(
    input rst_n,//reset: low effect
    input clk,
    input clk_user_input_control,// this is used to select tube
    input clk_user_input_time,// this is used to set time
    input confirm,// user confirmed time setting
    output reg [7:0] enable,
    output [7:0] seg_out0, //seg0 selection
    output [7:0] seg_out1, //segl selection
    output [31:0] sec_output,
    output [31:0] min_output,
    output [31:0] hour_output
    );
    
    // this part used to display distinct number
    parameter period =250000;
    reg [31:0] cnt; // used for clk->clk_out
    reg clk_out;
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
    
    parameter t0 = 8'b0000_0000,t1 = 8'b0000_0001,t2 = 8'b0000_0010,t3 = 8'b0000_0100,
        t4 = 8'b0000_1000,t5 = 8'b0001_0000,t6 = 8'b0010_0000,t7 = 8'b0100_0000,t8 = 8'b1000_0000;
    reg [3:0]scan_cnt;
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

    //select tube
    always @(scan_cnt)
    begin
        case (scan_cnt)
            4'b0000:enable = t1 ;
            4'b0001:enable = t2 ; 
            4'b0010:enable = t3 ; 
            4'b0011:enable = t4 ; 
            4'b0100:enable = t5 ;    
            4'b0101:enable = t6 ;    
            4'b0110:enable = t0 ; 
            4'b0111:enable = t8 ; 
            default:enable = t0 ; 
        endcase
    end
    time_setting ts1 (.scan_cnt(scan_cnt),.rst_n(rst_n),.time_setting(clk_user_input_time),
    .control(clk_user_input_control),.clk(clk),.confirm(confirm),.seg_out(seg_out0),.sec_output(sec_output),
    .min_output(min_output),.hour_output(hour_output));
    
    time_setting ts2 (.scan_cnt(scan_cnt),.rst_n(rst_n),.time_setting(clk_user_input_time),
    .control(clk_user_input_control),.clk(clk),.confirm(confirm),.seg_out(seg_out1));

endmodule
