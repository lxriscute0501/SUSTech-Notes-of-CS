`timescale 1ns / 1ps
module gesture_control (
    input clk,                // 系统时钟
    input rst_n,              // 复位信号，低电平有效
    input left_btn,           // 左键输入（模拟左手手势）
    input right_btn,          // 右键输入（模拟右手手势）
    input up_btn,             // 上键输入（增加倒计时）
    input down_btn,           // 下键输入（减少倒计时）
    input gesture_mode,       // 手势模式控制信号（1: 支持调整倒计时，0: 基础手势模拟）
    output reg power_on,      // 系统开机信号
    output reg power_off    // 系统关机信号
);


endmodule

