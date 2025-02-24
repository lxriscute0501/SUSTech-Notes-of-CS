`timescale 1ns / 1ps

module advanced_settings (
    input clk,                 // 系统时钟
    input rst_n,               // 复位信号，低电平有效   
    input advance_settings,  //进入高级设置(拨码开关)
    input mode_clean_btn,          // 智能提醒
    input mode_gesture_btn,       // 手势时间
    input mode_system_time_btn,  // 系统时间调整
    input time_btn,  // 增加时间调整按键
    input time_select_btn,
    input confirm_btn,         // 确认设置按键
    input initialize,
    output [7:0] seg_out0,        // 数码管显示
    output [7:0] seg_out1,         // 数码管显示
    output [7:0] seg_en,
    output reg [31:0] mode_1_time_hour,
    output reg [31:0] mode_1_time_min,
    output reg [31:0] mode_1_time_sec,
    output reg [31:0] mode_2_time_hour,
    output reg [31:0] mode_2_time_min,
    output reg [31:0] mode_2_time_sec,
    output reg [31:0] mode_3_time_hour,
    output reg [31:0] mode_3_time_min,
    output reg [31:0] mode_3_time_sec   
);

// 默认值定义
// 清洁模式
wire [31:0] DEFAULT_MODE_1_TIME_hour;
assign DEFAULT_MODE_1_TIME_hour = 10; 
wire [31:0] DEFAULT_MODE_1_TIME_min;
assign DEFAULT_MODE_1_TIME_min =0;
wire [31:0] DEFAULT_MODE_1_TIME_sec;
assign DEFAULT_MODE_1_TIME_sec = 0;

//手势
wire [31:0] DEFAULT_MODE_2_TIME_hour;
assign DEFAULT_MODE_2_TIME_hour = 0; 
wire[31:0]  DEFAULT_MODE_2_TIME_min;
assign DEFAULT_MODE_2_TIME_min = 0; 
wire[31:0]  DEFAULT_MODE_2_TIME_sec;
assign DEFAULT_MODE_2_TIME_sec = 5; 

// 系统时间
wire[31:0]  DEFAULT_MODE_3_TIME_hour; 
assign DEFAULT_MODE_3_TIME_hour = 0;  
wire [31:0] DEFAULT_MODE_3_TIME_min; 
assign DEFAULT_MODE_3_TIME_min = 0;  
wire [31:0] DEFAULT_MODE_3_TIME_sec; 
assign DEFAULT_MODE_3_TIME_sec = 0;  

wire [31:0] MODE_TIME_SETTING_sec;
wire [31:0] MODE_TIME_SETTING_min;
wire [31:0] MODE_TIME_SETTING_hour;


always @(*) begin
    if (advance_settings==1'b1 && initialize==1'b1) begin
        mode_1_time_hour = DEFAULT_MODE_1_TIME_hour;
        mode_1_time_min = DEFAULT_MODE_1_TIME_min;
        mode_1_time_sec = DEFAULT_MODE_1_TIME_sec;
        mode_2_time_hour = DEFAULT_MODE_2_TIME_hour;
        mode_2_time_min = DEFAULT_MODE_2_TIME_min;
        mode_2_time_sec = DEFAULT_MODE_2_TIME_sec;
        mode_3_time_hour = DEFAULT_MODE_3_TIME_hour;
        mode_3_time_min = DEFAULT_MODE_3_TIME_min;
        mode_3_time_sec = DEFAULT_MODE_3_TIME_sec;
    end
    else if(advance_settings==1'b1 && mode_clean_btn==1'b1) begin
        mode_1_time_hour = MODE_TIME_SETTING_hour;
        mode_1_time_min = MODE_TIME_SETTING_min;
        mode_1_time_sec = MODE_TIME_SETTING_sec;
    end
    else if(advance_settings==1'b1 && mode_gesture_btn==1'b1) begin
        mode_2_time_hour = MODE_TIME_SETTING_hour;
        mode_2_time_min = MODE_TIME_SETTING_min;
        mode_2_time_sec = MODE_TIME_SETTING_sec;
    end
    else if(advance_settings==1'b1 && mode_system_time_btn==1'b1) begin
        mode_3_time_hour = MODE_TIME_SETTING_hour;
        mode_3_time_min = MODE_TIME_SETTING_min;
        mode_3_time_sec = MODE_TIME_SETTING_sec;
    end
    else begin
        mode_1_time_hour = DEFAULT_MODE_1_TIME_hour;
        mode_1_time_min = DEFAULT_MODE_1_TIME_min;
        mode_1_time_sec = DEFAULT_MODE_1_TIME_sec;
        mode_2_time_hour = DEFAULT_MODE_2_TIME_hour;
        mode_2_time_min = DEFAULT_MODE_2_TIME_min;
        mode_2_time_sec = DEFAULT_MODE_2_TIME_sec;
        mode_3_time_hour = DEFAULT_MODE_3_TIME_hour;
        mode_3_time_min = DEFAULT_MODE_3_TIME_min;
        mode_3_time_sec = DEFAULT_MODE_3_TIME_sec;
    end
    
end


// 实例化 time_setting_controller 用于调整三个时间
time_setting_controller time_setting_controller (
    .rst_n(rst_n),
    .clk(clk),
    .clk_user_input_control(time_select_btn),  // 控制切换倒计时选择
    .clk_user_input_time(time_btn),  // 用于调节时间
    .confirm(confirm_btn),  // 确认选择
    .seg_out0(seg_out0),
    .seg_out1(seg_out1),
    .enable(seg_en),
    .sec_output(MODE_TIME_SETTING_sec),
    .min_output(MODE_TIME_SETTING_min),
    .hour_output(MODE_TIME_SETTING_hour)
);


endmodule