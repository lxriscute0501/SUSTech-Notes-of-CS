`timescale 1ns / 1ps

module advanced_settings (
    input clk,                 // ϵͳʱ��
    input rst_n,               // ��λ�źţ��͵�ƽ��Ч   
    input advance_settings,  //����߼�����(���뿪��)
    input mode_clean_btn,          // ��������
    input mode_gesture_btn,       // ����ʱ��
    input mode_system_time_btn,  // ϵͳʱ�����
    input time_btn,  // ����ʱ���������
    input time_select_btn,
    input confirm_btn,         // ȷ�����ð���
    input initialize,
    output [7:0] seg_out0,        // �������ʾ
    output [7:0] seg_out1,         // �������ʾ
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

// Ĭ��ֵ����
// ���ģʽ
wire [31:0] DEFAULT_MODE_1_TIME_hour;
assign DEFAULT_MODE_1_TIME_hour = 10; 
wire [31:0] DEFAULT_MODE_1_TIME_min;
assign DEFAULT_MODE_1_TIME_min =0;
wire [31:0] DEFAULT_MODE_1_TIME_sec;
assign DEFAULT_MODE_1_TIME_sec = 0;

//����
wire [31:0] DEFAULT_MODE_2_TIME_hour;
assign DEFAULT_MODE_2_TIME_hour = 0; 
wire[31:0]  DEFAULT_MODE_2_TIME_min;
assign DEFAULT_MODE_2_TIME_min = 0; 
wire[31:0]  DEFAULT_MODE_2_TIME_sec;
assign DEFAULT_MODE_2_TIME_sec = 5; 

// ϵͳʱ��
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


// ʵ���� time_setting_controller ���ڵ�������ʱ��
time_setting_controller time_setting_controller (
    .rst_n(rst_n),
    .clk(clk),
    .clk_user_input_control(time_select_btn),  // �����л�����ʱѡ��
    .clk_user_input_time(time_btn),  // ���ڵ���ʱ��
    .confirm(confirm_btn),  // ȷ��ѡ��
    .seg_out0(seg_out0),
    .seg_out1(seg_out1),
    .enable(seg_en),
    .sec_output(MODE_TIME_SETTING_sec),
    .min_output(MODE_TIME_SETTING_min),
    .hour_output(MODE_TIME_SETTING_hour)
);


endmodule