`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/23 10:16:22
// Design Name: 
// Module Name: t_tb
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


module menu_module_tb;

    // Declare signals for the inputs
    reg clk;                         // 系统时钟
    reg menu_btn;                    // 菜单按键
    reg clean_btn;                   // 自清洁模式
    reg light_btn;                   // 照明开关
    reg hand_left;                   // 左手势
    reg hand_right;                  // 右手势
    reg up_toward;                   // 上按键
    reg mid_toward;                  // 中间按键
    reg down_toward;                 // 下按键
    reg advance_settings;            // 高级设置开关
    reg power_on;                    // 开关机
    reg initialize;                  // 恢复出厂设置
    reg advanced_clean;              // 高级自清洁
    reg advanced_gesture;            // 高级手势识别
    reg advanced_system_time;        // 高级系统时间
    reg clean_by_hand;               // 手动清零工作时间
    reg search_for_working_time;     // 搜索工作时间
    reg search_for_figure;           // 搜索图形
    reg open;                        // 测试开关
    reg close;                       // 测试关闭

    // Declare signals for the outputs
    wire light_on;                   // 照明状态
    wire [5:0] mode;                 // 当前模式
    wire intelligent_reminder;       // 智能提醒
    wire [7:0] seg_en;               // 数码管使能
    wire [7:0] seg_out0;             // 数码管输出
    wire [7:0] seg_out1;
    wire sound;                      // 声音输出

    // Instantiate the menu_module
    menu_module uut (
        .clk(clk),
        .menu_btn(menu_btn),
        .clean_btn(clean_btn),
        .light_btn(light_btn),
        .hand_left(hand_left),
        .hand_right(hand_right),
        .up_toward(up_toward),
        .mid_toward(mid_toward),
        .down_toward(down_toward),
        .advance_settings(advance_settings),
        .power_on(power_on),
        .initialize(initialize),
        .advanced_clean(advanced_clean),
        .advanced_gesture(advanced_gesture),
        .advanced_system_time(advanced_system_time),
        .clean_by_hand(clean_by_hand),
        .search_for_working_time(search_for_working_time),
        .search_for_figure(search_for_figure),
        .open(open),
        .close(close),
        .light_on(light_on),
        .mode(mode),
        .intelligent_reminder(intelligent_reminder),
        .seg_en(seg_en),
        .seg_out0(seg_out0),
        .seg_out1(seg_out1),
        .sound(sound)
    );


    always begin
        #5 clk = ~clk;  
    end

    
    initial begin
      
        clk = 0;
        menu_btn = 0;
        clean_btn = 0;
        light_btn = 0;
        hand_left = 0;
        hand_right = 0;
        up_toward = 0;
        mid_toward = 0;
        down_toward = 0;
        advance_settings = 0;
        power_on = 0;
        initialize = 0;
        advanced_clean = 0;
        advanced_gesture = 0;
        advanced_system_time = 0;
        clean_by_hand = 0;
        search_for_working_time = 0;
        search_for_figure = 0;
        open = 0;
        close = 0;


        #10 open = 1; 
        

        #20 menu_btn = 1; 
        #40 menu_btn = 0;  

        
        #60 up_toward = 1;
        #120  up_toward=0;
        #130 mid_toward = 1;
        #190 mid_toward =0;
      
        #200 menu_btn = 1;
        #210 menu_btn = 0;

        #220 menu_btn = 1;
        #230 menu_btn = 0;
        #240 down_toward =1;
        #290 down_toward =0;


        #600 $finish;
    end

    // Monitor signals
    initial begin
        $monitor("Time=%0t, mode=%b, light_on=%b, seg_en=%b, seg_out0=%b, seg_out1=%b, sound=%b",
                 $time, mode, light_on, seg_en, seg_out0, seg_out1, sound);
    end

endmodule

