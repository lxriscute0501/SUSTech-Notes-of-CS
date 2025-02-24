`timescale 1ns / 1ps

module menu_module(
    input clk,           // 系统时钟
    input menu_btn,     // 菜单按键 P5
    input clean_btn,     // 自清洁模式(拨码开关) T5
    input light_btn,     // 照明(拨码开关) U3
    input hand_left,     // 左手势 V1
    input hand_right,    // 右手势 R11
    input up_toward,  //上按键 J4
    input mid_toward,  //中间按键 R15
    input down_toward,   //下按键 R17
    input advance_settings,//高级设置(拨码开关)P3
    input power_on,  // 开关机//s6 P15
    input initialize,//恢复出厂设置(拨码开关)P4
    input advanced_clean,//(拨码开关)P2
    input advanced_gesture,//(拨码开关)R2
    input advanced_system_time,//(拨码开关)M4
    input clean_by_hand,//V5
    input search_for_working_time,//N4
    input search_for_figure, //R1
    input open,//test//////////////////////////////////////////////////////T3
    input close,//test////////////////////////////////////////////////R3
    output reg light_on, // 照明状态 (LED) K1
    //F6 G4 G3 J4 H4 J3 
    output reg [5:0] mode, // 当前模式 [0:待机, 1:抽油烟1档, 2:抽油烟2档, 3:抽油烟3档, 4:自清洁, 5:高级设置]
    output intelligent_reminder,// K2
    output reg [7:0] seg_en, // 数码管使能信号
    output reg [7:0] seg_out0, // 数码管输出信号
    output reg [7:0] seg_out1,
    output sound
);
wire rst_n;
reg reg_rst_n;
assign rst_n = (reg_rst_n)?1'b1:1'b0;

//这部分是档位按键
wire mode1,mode2,mode3;
wire m1,m2,m3;

//这部分是高级设置中的按键
wire up,mid,down;


// 使用寄存器存储按键信号，以便更好地控制
reg up_reg, mid_reg, down_reg;
reg m1_reg, m2_reg, m3_reg;
always @(posedge clk) begin
    if (advance_settings) begin
        up_reg <= up_toward;   
        mid_reg <= mid_toward;
        down_reg <= down_toward;
    end else begin
        m1_reg <= up_toward;
        m2_reg <= mid_toward;
        m3_reg <= down_toward;
    end
end

// 输出控制
assign up = up_reg;
assign mid = mid_reg;
assign down = down_reg;
assign mode1 = m1_reg;
assign mode2 = m2_reg;
assign mode3 = m3_reg;

wire menu_btn_mid;
wire clean_btn_mid;
wire mid1,mid2,mid3;
denouncer de1(.old_input(mode1),.new_input(mid1),.clk(clk),.rst_n(rst_n));
denouncer de2(.old_input(mode2),.new_input(mid2),.clk(clk),.rst_n(rst_n));
denouncer de3(.old_input(mode3),.new_input(mid3),.clk(clk),.rst_n(rst_n));
trig_test tt2(.clk(clk),.rst_n(rst_n),.trig(mid1),.pos(m1));
trig_test tt3(.clk(clk),.rst_n(rst_n),.trig(mid2),.pos(m2));
trig_test tt4(.clk(clk),.rst_n(rst_n),.trig(mid3),.pos(m3));

//assign mid1=mode1 ;
//assign mid2 =mode2 ;
//assign mid3 =mode3 ;
wire advance_settings_mid;
trig_test tt(.clk(clk),.rst_n(rst_n),.trig(menu_btn),.pos(menu_btn_mid));
trig_test tt1(.clk(clk),.rst_n(rst_n),.trig(clean_btn),.pos(clean_btn_mid));
trig_test tt5(.clk(clk),.rst_n(rst_n),.trig(advance_settings),.pos(advance_settings_mid));
//这是照明部分
always @(*) begin
    if (open) begin
        light_on=light_btn;
    end else begin
        light_on=1'b0;
    end
end

//状态定义 s0待机 s1一档 s2二挡 s3三挡 s4自清洁 s5高级设置 s关机 ss菜单
parameter s0 = 6'b100000 , s1 = 6'b010000 , s2 = 6'b001000 , s3 = 6'b000100 , s4 = 6'b000010 , s5 = 6'b000001;
parameter s = 6'b000000;
parameter ss =6'b111111;

// system_time 模块  时间显示
wire system_display;
wire system_time_set;
wire [7:0] system_seg_en_output;
wire [7:0]system_seg_out0_output;
wire [7:0]system_seg_out1_output;
assign system_display = 1'b1;
//working_time模块中需要用到的时间
wire working_show; 
wire working_reset;
wire working_stop;
wire [7:0]working_seg_en_output; 
wire [7:0]working_seg_out0_output;
wire [7:0]working_seg_out1_output; 
assign working_show = 1'b1;
//count_down模块中需要的线
wire count_down_display;
wire count_down_time_set;
wire [31:0]count_down_sec_input;
wire [31:0]count_down_min_input;
wire [31:0]count_down_hour_input;
wire [7:0]count_down_seg_en_output;
wire [7:0]count_down_seg_out0_output;
wire [7:0]count_down_seg_out1_output;
wire count_down_time_out;
assign count_down_display = 1'b1;
//advanced_settings输入输出线
wire advanced_reset;
wire [7:0]advanced_seg_en;
wire [7:0]advanced_seg_out0;
wire [7:0]advanced_seg_out1;
wire [31:0]advanced_mode1_sec;
wire [31:0]advanced_mode1_min;
wire [31:0]advanced_mode1_hour;
wire [31:0]advanced_mode2_sec;
wire [31:0]advanced_mode2_min;
wire [31:0]advanced_mode2_hour;
wire [31:0]advanced_mode3_sec;
wire [31:0]advanced_mode3_min;
wire [31:0]advanced_mode3_hour;
assign advanced_reset = 1'b1;

wire [7:0]show_time_seg_en;
wire [7:0]show_time_seg_out0;
wire [7:0]show_time_seg_out1;

reg clean_start;

reg system_time_reset;
reg working_time_rst;
reg working_time_stop;


//wire close;////////////////////////////////////////////////////////////
//wire open;////////////////////////////////////////////////////////////

reg [5:0] next_mode;
reg count_down_set;
assign count_down_time_set = (count_down_set) ? 1'b1 : 1'b0;
reg mode_is_s4;
reg i3;
reg i4;
reg menu_checked;
reg mode3_used_out;


always @(*) begin 
    case (mode)
        s: begin
            menu_checked=1'b0;
            mode3_used_out = 1'b0;
            next_mode=s0;
            reg_rst_n=1'b0;
            seg_en = 8'b0000_0000;
            seg_out0 = 8'b0000_0000;
            seg_out1 = 8'b0000_0000;
            //working_time system_time (done)
            system_time_reset = 1'b1;
            working_time_rst = 1'b0; // 清零操作
            working_time_stop = 1'b1;         
        end
        s0: begin
            menu_checked=1'b0;
            mode3_used_out = 1'b0;
            reg_rst_n=1'b1;
            //系统继续计时
            system_time_reset = 1'b0;
            //工作不计时间
            working_time_stop = 1'b1;
            working_time_rst = 1'b1;
            if (search_for_working_time) begin
                seg_en=working_seg_en_output;
                seg_out0=working_seg_out0_output;
                seg_out1=working_seg_out1_output;
            end else if(search_for_figure) begin
                seg_en=show_time_seg_en;
                seg_out0=show_time_seg_out0;
                seg_out1=show_time_seg_out1;
            end else begin
                seg_en = system_seg_en_output;
                seg_out0 = system_seg_out0_output;
                seg_out1 = system_seg_out1_output;
                //待机模式下，手动对工作时间清零
                if (clean_by_hand) begin
                    working_time_rst = 1'b0; // 清零操作
                end
            end
            
            
            if (~open) begin
                next_mode=s;
            end else if(menu_btn_mid) begin
                next_mode=ss;
            end else begin
                next_mode=s0;
            end

        end
        ss: begin
            menu_checked=1'b0;
            mode3_used_out = 1'b0;
            system_time_reset = 1'b0;
            count_down_set =1'b0; 
            working_time_stop = 1'b1;
            working_time_rst = 1'b1;
            seg_en = system_seg_en_output;
            seg_out0 = system_seg_out0_output;
            seg_out1 = system_seg_out1_output;
            if (~open) begin
                next_mode =s;
            end else if(m1) begin
                next_mode =s1;
            end else if(m2) begin
                next_mode =s2;
            end else if(m3 && mode3_used_out==1'b0) begin
                next_mode =s3;
                count_down_set =1'b1;
                mode_is_s4 = 1'b0;
            end else if(clean_btn_mid) begin
                next_mode =s4;
                mode_is_s4 = 1'b1;
                count_down_set =1'b1;
            end else if(advance_settings_mid ) begin
                next_mode =s5;
            end else begin
                next_mode =ss;
            end
        end
        s1: begin
            menu_checked=1'b0;
            mode3_used_out = 1'b0;
            system_time_reset = 1'b0;
            count_down_set =1'b0; 
            working_time_rst = 1'b1;
            working_time_stop = 1'b0;
            seg_en = system_seg_en_output;
            seg_out0 = system_seg_out0_output;
            seg_out1 = system_seg_out1_output;
            if(m2) begin
                next_mode =s2;
            end else if(menu_btn_mid)begin
                next_mode =s0;
            end else begin
                next_mode =s1;
            end
        end
        s2: begin
            menu_checked=1'b0;
            mode3_used_out = 1'b0;
            system_time_reset = 1'b0;
            count_down_set =1'b0; 
            working_time_rst = 1'b1;
            working_time_stop = 1'b0;
            seg_en = system_seg_en_output;
            seg_out0 = system_seg_out0_output;
            seg_out1 = system_seg_out1_output;
            if(m1) begin
                next_mode =s1;
            end else if(menu_btn_mid)begin
                next_mode =s0;
            end else begin
                next_mode =s2;
            end
        end
        s3: begin
            system_time_reset = 1'b0;

            working_time_rst = 1'b1;
            working_time_stop = 1'b0;

            seg_en = count_down_seg_en_output;
            seg_out0 = count_down_seg_out0_output;
            seg_out1 = count_down_seg_out1_output;
            
             
            count_down_set =1'b0;  
            
            if(menu_btn_mid && count_down_time_out ==1'b0)begin
                menu_checked=1'b1;
                mode_is_s4 = 1'b0;
                count_down_set =1'b1;            
            end 

            if(menu_checked==1'b1 && count_down_time_out ==1'b1) begin
                next_mode=s0;
                mode3_used_out = 1'b1;
            end else if(menu_checked==1'b0 && count_down_time_out ==1'b1) begin
                next_mode=s2;
                mode3_used_out = 1'b1;
            end else begin
                next_mode=s3;
            end
        
        end
        s4: begin
            system_time_reset = 1'b0;
             
            working_time_rst = 1'b1;
            working_time_stop = 1'b1;

            seg_en = count_down_seg_en_output;
            seg_out0 = count_down_seg_out0_output;
            seg_out1 = count_down_seg_out1_output;
            //count down
            
            count_down_set =1'b0;  
            mode_is_s4=1'b1;
            
            if (count_down_time_out) begin
                working_time_rst = 1'b0; // 清零操作
                working_time_stop = 1'b1;
                next_mode=s0;
            end else begin
                next_mode=s4;
            end
        end
        s5: begin
            system_time_reset = 1'b0;
            working_time_rst = 1'b1;
            working_time_stop = 1'b1;

            seg_en = advanced_seg_en;
            seg_out0 = advanced_seg_out0;
            seg_out1 = advanced_seg_out1;
            if (menu_btn_mid) begin
                system_time_reset = 1'b1;//设置值
                next_mode=s0;
            end
        end
        
            
    endcase

end

always @(posedge clk,negedge open ) begin//, negedge close
    if (~open) begin
        mode<=s;
    end else
    begin
        mode<=next_mode;
    end
end


assign system_time_set = (system_time_reset)? 1'b1 : 1'b0;

assign working_reset = (working_time_rst) ? 1'b1 : 1'b0;
assign working_stop = (working_time_stop) ? 1'b1 : 1'b0;






assign count_down_sec_input = (mode_is_s4)? 0 : 0;
assign count_down_min_input = (mode_is_s4)? 3 : 1;
assign count_down_hour_input = (mode_is_s4)? 0 : 0;




// 实例化 count_down 模块，用于飓风模式和自清洁模式倒计时
count_down count_down_inst (
   .rst_n(count_down_display),
   .clk(clk),
   .time_set(count_down_time_set),
   .sec_input(count_down_sec_input),
   .min_input(count_down_min_input),
   .hour_input(count_down_hour_input),
   .seg_en(count_down_seg_en_output),
   .seg_out0(count_down_seg_out0_output),
   .seg_out1(count_down_seg_out1_output),
   .time_out(count_down_time_out)
);


// 实例化 working_time 模块，用于记录工作时间
working_time working_time_inst (
   .show(working_show),
   .rst_n(working_reset),
   .clk(clk),
   .sec_input(advanced_mode1_sec),
   .min_input(advanced_mode1_min),
   .hour_input(advanced_mode1_hour),
   .stop(working_stop),
   .seg_en(working_seg_en_output),
   .seg_out0(working_seg_out0_output),
   .seg_out1(working_seg_out1_output),
   .time_out(intelligent_reminder)
);


system_time system_time_inst (
   .rst_n(system_display),
   .clk(clk),
   .time_set(system_time_set),
   .sec_input(advanced_mode3_sec),
   .min_input(advanced_mode3_min),
   .hour_input(advanced_mode3_hour),
   .seg_en(system_seg_en_output),
   .seg_out0(system_seg_out0_output),
   .seg_out1(system_seg_out1_output)
);


// 实例化 advanced_settings 模块
advanced_settings advanced_settings_inst (
   .clk(clk),
   .rst_n(advanced_reset),
   .advance_settings(advance_settings),
   .mode_clean_btn(advanced_clean),
   .mode_gesture_btn(advanced_gesture),
   .mode_system_time_btn(advanced_system_time),
   .time_btn(up),
   .time_select_btn(down),
   .confirm_btn(mid),
   .initialize(initialize),
   .mode_1_time_hour(advanced_mode1_hour),
   .mode_1_time_min(advanced_mode1_min),
   .mode_1_time_sec(advanced_mode1_sec),
   .mode_2_time_hour(advanced_mode2_hour),
   .mode_2_time_min(advanced_mode2_min),
   .mode_2_time_sec(advanced_mode2_sec),
   .mode_3_time_hour(advanced_mode3_hour),
   .mode_3_time_min(advanced_mode3_min),
   .mode_3_time_sec(advanced_mode3_sec),
   .seg_out0(advanced_seg_out0),
   .seg_out1(advanced_seg_out1),
   .seg_en(advanced_seg_en)
);

show_time show_time_inst(
    .clk(clk),
    .rst_n(rst_n),
    .sec_input(advanced_mode2_sec),
    .min_input(advanced_mode2_min),
    .hour_input(advanced_mode2_hour),
    .seg_en(show_time_seg_en),
    .seg_out0(show_time_seg_out0),
    .seg_out1(show_time_seg_out1)
);

wire sound_control;


sound_player sound_player_inst (
    .clk(clk),
    .rst_n(sound_control),
    .sound(sound)
);

endmodule