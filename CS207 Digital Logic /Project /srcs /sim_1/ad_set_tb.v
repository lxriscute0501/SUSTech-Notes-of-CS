`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/23 08:52:35
// Design Name: 
// Module Name: ad_set_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ad_set_tb;

  // Declare inputs as reg (for driving values in the testbench)
  reg clk;
  reg rst_n;
  reg advance_settings;
  reg mode_clean_btn;
  reg mode_gesture_btn;
  reg mode_system_time_btn;
  reg time_btn;
  reg time_select_btn;
  reg confirm_btn;
  reg initialize;

  // Declare outputs as wire (for monitoring outputs from the DUT)
  wire in_settings_mode;
  wire [7:0] seg_out0;
  wire [7:0] seg_out1;
  wire [7:0] seg_en;
  wire mode_1_time_hour;
  wire mode_1_time_min;
  wire mode_1_time_sec;
  wire mode_2_time_hour;
  wire mode_2_time_min;
  wire mode_2_time_sec;
  wire mode_3_time_hour;
  wire mode_3_time_min;
  wire mode_3_time_sec;

  // Instantiate the DUT (Device Under Test)
  advanced_settings uut (
    .clk(clk),
    .rst_n(rst_n),
    .advance_settings(advance_settings),
    .mode_clean_btn(mode_clean_btn),
    .mode_gesture_btn(mode_gesture_btn),
    .mode_system_time_btn(mode_system_time_btn),
    .time_btn(time_btn),
    .time_select_btn(time_select_btn),
    .confirm_btn(confirm_btn),
    .initialize(initialize),
    .in_settings_mode(in_settings_mode),
    .seg_out0(seg_out0),
    .seg_out1(seg_out1),
    .seg_en(seg_en),
    .mode_1_time_hour(mode_1_time_hour),
    .mode_1_time_min(mode_1_time_min),
    .mode_1_time_sec(mode_1_time_sec),
    .mode_2_time_hour(mode_2_time_hour),
    .mode_2_time_min(mode_2_time_min),
    .mode_2_time_sec(mode_2_time_sec),
    .mode_3_time_hour(mode_3_time_hour),
    .mode_3_time_min(mode_3_time_min),
    .mode_3_time_sec(mode_3_time_sec)
  );

  // Clock generation
  always begin
    #5 clk = ~clk;  // Generate a clock with a period of 10 time units
  end

  // Initial block to drive inputs and simulate the test
  initial begin
    // Initialize inputs
    clk = 0;
    rst_n = 0;
    advance_settings = 0;
    mode_clean_btn = 0;
    mode_gesture_btn = 0;
    mode_system_time_btn = 0;
    time_btn = 0;
    time_select_btn = 0;
    confirm_btn = 0;
    initialize = 0;

    // Reset the DUT
    #10 rst_n = 1;  // Apply reset (active low)
    #10 rst_n = 0;  // Release reset
    #10 rst_n = 1;

    // Test 1: Enter advanced settings mode
    advance_settings = 1;
    


    // Test 2: Press mode buttons
    mode_clean_btn = 1;
    #10 mode_clean_btn = 0;
    mode_gesture_btn = 1;
    #10 mode_gesture_btn = 0;
    mode_system_time_btn = 1;
    #10 mode_system_time_btn = 0;

    // Test 3: Press time adjustment buttons
    time_btn = 1;
    #10 time_btn = 0;
    time_select_btn = 1;
    #10 time_select_btn = 0;

    // Test 4: Press confirm button
    confirm_btn = 1;
    #10 confirm_btn = 0;

    // Test 5: Initialize the settings
    initialize = 1;
    #10 initialize = 0;

    #120 advance_settings = 0;

    // Test 6: Checking the system after inputs are set
    #20;
    $display("in_settings_mode = %b", in_settings_mode);
    $display("seg_out0 = %h, seg_out1 = %h", seg_out0, seg_out1);
    $display("mode_1_time: %d:%d:%d", mode_1_time_hour, mode_1_time_min, mode_1_time_sec);
    $display("mode_2_time: %d:%d:%d", mode_2_time_hour, mode_2_time_min, mode_2_time_sec);
    $display("mode_3_time: %d:%d:%d", mode_3_time_hour, mode_3_time_min, mode_3_time_sec);

    // End the simulation
    $finish;
  end

endmodule

