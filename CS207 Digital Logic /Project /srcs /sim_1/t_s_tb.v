`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/23 08:59:57
// Design Name: 
// Module Name: t_s_tb
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


module time_setting_controller_tb;

  // Declare inputs as reg (for driving values in the testbench)
  reg rst_n;
  reg clk;
  reg clk_user_input_control;
  reg clk_user_input_time;
  reg confirm;

  // Declare outputs as wire (for monitoring outputs from the DUT)
  wire [7:0] enable;
  wire [7:0] seg_out0;
  wire [7:0] seg_out1;
  wire [31:0] sec_output;
  wire [31:0] min_output;
  wire [31:0] hour_output;

  // Instantiate the DUT (Device Under Test)
  time_setting_controller uut (
    .rst_n(rst_n),
    .clk(clk),
    .clk_user_input_control(clk_user_input_control),
    .clk_user_input_time(clk_user_input_time),
    .confirm(confirm),
    .enable(enable),
    .seg_out0(seg_out0),
    .seg_out1(seg_out1),
    .sec_output(sec_output),
    .min_output(min_output),
    .hour_output(hour_output)
  );

  // Clock generation
  always begin
    #5 clk = ~clk;  // Generate a clock with a period of 10 time units
  end

  // Testbench signals generation
  initial begin
    // Initialize inputs
    clk = 0;
    rst_n = 0;
    clk_user_input_control = 0;
    clk_user_input_time = 0;
    confirm = 0;

    // Reset the DUT
    #10 rst_n = 1;  // Apply reset (active low)
    #10 rst_n = 0;  // Release reset
    #10 rst_n = 1;

    // Test 1: Entering time setting mode
    // Simulate user selecting the time tube (e.g., hours, minutes, seconds)
    clk_user_input_control = 1;  // User selects control signal to choose the tube (e.g., hours)
    #10 clk_user_input_control = 0;
    
    // Test 2: Time setting (e.g., set hours)
    clk_user_input_time = 1;  // User inputs time (increase value)
    #10 clk_user_input_time = 0; // Turn off the input signal
    
    // Test 3: Set minutes
    clk_user_input_control = 1;  // Select minutes tube
    #10 clk_user_input_control = 0;
    
    clk_user_input_time = 1;  // User inputs minutes
    #10 clk_user_input_time = 0;

    
    // Test 4: Set seconds
    
    clk_user_input_control = 1;  // Select seconds tube
    #10 clk_user_input_control = 0;
    
    clk_user_input_time = 1;  // User inputs seconds
    #10 clk_user_input_time = 0;

    // Test 5: Confirm the time setting
    confirm = 1;  // User confirms the time setting
    #10 confirm = 0;

    // Test 6: Check output values
    #20;
    $display("Enable = %b", enable);
    $display("seg_out0 = %h, seg_out1 = %h", seg_out0, seg_out1);
    $display("sec_output = %d", sec_output);
    $display("min_output = %d", min_output);
    $display("hour_output = %d", hour_output);

    // End the simulation
    $finish;
  end

endmodule

