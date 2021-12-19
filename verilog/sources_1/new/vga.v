// set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets I_clk_IBUF]


module vga(

    // 控制魔方
    input                       I_UD,
    input                       I_LR,
    input                       I_FB,
    input                       I_Mz,
    input                       I_rst_cube,
    
    // 控制计时器
    input                       I_timer_rst,
    input                       I_timer_pau,
    output  reg     [7:0]       O_timer,        // led显示
    output  reg     [7:0]       O_select,       // led选择
    
    input                       I_shift_cube,
    input           [1:0]       I_num_cube,
    
    input                       I_bluetooth_get,
    
    input                       I_clk   ,       // 时钟 100MHz
    input                       I_rst_n ,
    output  reg     [3:0]       O_red   ,
    output  reg     [3:0]       O_green ,
    output  reg     [3:0]       O_blue  ,
    output                      O_hs    ,       // 行同步
    output                      O_vs    ,        // 场同步（列）
    output  led_clk,
    output  led_rst,
    output  led_hs,
    output  led_vs,
    
    output led_1,
    output led_2,
    output [6:0] leds
);



//  1024*768 行时序
parameter           H_SYNC      =       11'd136;    // 同步
parameter           H_BACK      =       11'd120;    // back_porch
parameter           H_DISP      =       11'd1024;
parameter           H_FRONT     =       11'd64;     // front_porch
parameter           H_TOTAL     =       11'd1344; 


// 1024*768 场时序（列）
parameter           V_SYNC      =       11'd6;
parameter           V_BACK      =       11'd29;
parameter           V_DISP      =       11'd768;
parameter           V_FRONT     =       11'd3;
parameter           V_TOTAL     =       11'd806;
wire clk_65M;
reg clk_1Hz;
reg clk_1000Hz;


wire rst__;
assign rst__ = ~I_rst_n;

reg [10:0] hcnt = 11'b0;    // 行时序计数器
reg [10:0] vcnt = 11'b0;    // 场时序计数器（列）

reg     [10:0]      cur_x;
reg     [10:0]      cur_y;

wire W_active_flag; // 激活标志，为 1 时收到的 RGB 数据可以显示出来

reg [1:0] flag_timer;   // led显示用


always @(posedge clk_65M) begin
    cur_x <= (hcnt < H_SYNC + H_BACK) ? 11'b0 : (hcnt - H_SYNC - H_BACK);
    cur_y <= (vcnt < V_SYNC + V_BACK) ? 11'b0 : (vcnt - V_SYNC - V_BACK);
end

always @(posedge clk_1000Hz) begin
    // 循环显示 led
    case (flag_timer)
        2'b00:  flag_timer <= 2'b01;
        2'b01:  flag_timer <= 2'b10;
        default:flag_timer <= 2'b00;
    endcase
end

assign led_clk = clk_65M;
assign led_rst = I_rst_n;
assign led_hs = O_hs;
assign led_vs = O_vs;

//=====================================================================
//=====================================================================

// 由 100Mhz 产生 1Hz、100Hz 的计时器时钟
reg [31:0] cnt_1Hz;
reg [31:0] cnt_1000Hz;
initial begin
    cnt_1Hz = 0;
    clk_1Hz = 32'b0;
    cnt_1000Hz = 32'b0; 
end
always @(posedge I_clk) begin
    if (cnt_1Hz == 32'd50000000) begin
        cnt_1Hz <= 32'b0;
        clk_1Hz <= ~clk_1Hz;        
    end
    else begin
        cnt_1Hz <= (cnt_1Hz + 32'b1);
    end
    
    if (cnt_1000Hz == 32'd100000) begin
        cnt_1000Hz <= 32'b0;
        clk_1000Hz <= ~clk_1000Hz;
    end
    else begin
        cnt_1000Hz <= (cnt_1000Hz + 32'b1);
    end
end

// 由 100Mhz 产生 65Mhz 的像素时钟
clk_wiz_1 clkwiz_haha(
    .clk_in1(I_clk),
    .clk_out1(clk_65M),
    .reset(rst__)
);
/*
reg [31:0] cnt_clk_65M = 32'b0;
always @(posedge I_clk or negedge I_rst_n) begin
    if (!I_rst_n)
        cnt_clk_65M <= 32'b0;
    else begin
        cnt_clk_65M = cnt_clk_65M + 32'd2791728742;
    end
end
assign clk_65M = (cnt_clk_65M <= 32'h7fffffff) ? 1'b0 : 1'b1;
*/

// 行同步信号发生器
always @(posedge clk_65M or negedge I_rst_n) begin
    if (!I_rst_n)
        hcnt <= 11'd0;
    else begin
        if (hcnt < H_TOTAL - 1'b1)
            hcnt <= hcnt + 1'b1;
        else
            hcnt <= 11'd0;
    end
end
assign O_hs = (hcnt < H_SYNC) ? 1'b0 : 1'b1;


// 场同步信号发生器
always @(posedge clk_65M or negedge I_rst_n) begin
    if (!I_rst_n)
        vcnt <= 11'd0;
    else begin
        if (hcnt == H_TOTAL - 1'b1) begin
            if (vcnt < V_TOTAL - 1'b1)
                vcnt <= vcnt + 1'b1;
            else
                vcnt <= 11'd0;
        end
        else
            vcnt <= vcnt;
    end
end
assign O_vs = (vcnt < V_SYNC) ? 1'b0 : 1'b1;


// 激活标志
assign W_active_flag =  (hcnt >= (H_SYNC + H_BACK           ))  &&
                        (hcnt <= (H_SYNC + H_BACK + H_DISP  ))  &&
                        (vcnt >= (V_SYNC + V_BACK           ))  &&
                        (vcnt <= (V_SYNC + V_BACK + V_DISP  ))  ;




parameter COL_U = 12'hfff;
parameter COL_L = 12'hf80;
parameter COL_F = 12'h0f0;
parameter COL_R = 12'hf00;
parameter COL_D = 12'hff0;
parameter COL_B = 12'h00f;
wire [11:0] COL_CUBE[0:5] = {COL_U, COL_L, COL_F, COL_R, COL_D, COL_B};



// 蓝牙模块
wire [7:0] cube_bluetooth_mode;
wire    cube_bluetooth_submit;
bluetooth bluetooth_ctrl (
    .clk(I_clk),
    .get(I_bluetooth_get),
    .out(cube_bluetooth_mode),
    .submit(cube_bluetooth_submit)
);

// 按钮控制虚拟魔方
wire [1:0] I_num_cube_tmp;
assign I_num_cube_tmp[1:0] = I_num_cube[1:0];
wire cube_bc_act, cube_bc_rst;
wire [1:0] cube_bc_num;
wire [3:0] cube_bc_mode;
buttons_ctrl_cube cube_bc (
    .clk(clk_65M),
    .I_UD(I_UD),
    .I_LR(I_LR),
    .I_FB(I_FB),
    .I_Mz(I_Mz),
    .I_rst(I_rst_cube),
    .I_shift(I_shift_cube),
    .I_num(I_num_cube_tmp),
    .I_mode(cube_bluetooth_mode),
    .I_bluetooth_submit(cube_bluetooth_submit),
    .O_act(cube_bc_act),
    .O_rst(cube_bc_rst),
    .O_num(cube_bc_num),
    .O_mode(cube_bc_mode),
    
    .tmpp(led_2)
);


reg [15:0] tttttmp=0;
always @(posedge cube_bluetooth_submit) begin
    tttttmp <= tttttmp + 1;
end

assign led_1 = tttttmp[0];
//assign led_2 = tttmpblue;
//assign leds[3:2] = cube_bc_mode[3:2];
//assign leds[1:0] = cube_bluetooth_mode[1:0];
assign leds[3:0] = cube_bc_mode;
assign leds[5:4] = cube_bc_num;
assign leds[6] = I_shift_cube;
//assign leds = I_num_cube_tmp;


// 虚拟魔方模块实例化
wire [11:0]     cube [0:5][0:8];
cube_ctrl cube_mod (
    .I_act(cube_bc_act),
    .I_mode(cube_bc_mode),
    .I_num(cube_bc_num),
    .I_rst(cube_bc_rst),
    .O({
        cube[5][8],     cube[5][7],     cube[5][6],     cube[5][5],     cube[5][4],     cube[5][3],     cube[5][2],     cube[5][1],     cube[5][0], 
        cube[4][8],     cube[4][7],     cube[4][6],     cube[4][5],     cube[4][4],     cube[4][3],     cube[4][2],     cube[4][1],     cube[4][0],
        cube[3][8],     cube[3][7],     cube[3][6],     cube[3][5],     cube[3][4],     cube[3][3],     cube[3][2],     cube[3][1],     cube[3][0],
        cube[2][8],     cube[2][7],     cube[2][6],     cube[2][5],     cube[2][4],     cube[2][3],     cube[2][2],     cube[2][1],     cube[2][0], 
        cube[1][8],     cube[1][7],     cube[1][6],     cube[1][5],     cube[1][4],     cube[1][3],     cube[1][2],     cube[1][1],     cube[1][0],
        cube[0][8],     cube[0][7],     cube[0][6],     cube[0][5],     cube[0][4],     cube[0][3],     cube[0][2],     cube[0][1],     cube[0][0]
    })
);




// 计时器实例化
wire [7:0] cbtm_s0;
wire [7:0] cbtm_s1;
wire [7:0] cbtm_m0;
cube_timer cb_timer(
    .clk_1Hz(clk_1Hz),
    .I_reset(I_timer_rst),
    .I_pause(~I_timer_pau),
    .O_leds_s0(cbtm_s0),
    .O_leds_s1(cbtm_s1),
    .O_leds_m0(cbtm_m0)
);


/*
reg [11:0] cube [0:5][0:8] = {
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U},
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U},
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U},
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U},
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U},
    {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}, {COL_U}
};
*/



/*
always @(negedge I_rst_n) begin
    {cube[0][0], cube[0][1], cube[0][2], cube[0][3], cube[0][4], cube[0][5], cube[0][6], cube[0][7], cube[0][8]} <= {COL_CUBE[2], COL_CUBE[0], COL_CUBE[5], COL_CUBE[0], COL_CUBE[0], COL_CUBE[0], COL_CUBE[0], COL_CUBE[0], COL_CUBE[1]};
    {cube[1][0], cube[1][1], cube[1][2], cube[1][3], cube[1][4], cube[1][5], cube[1][6], cube[1][7], cube[1][8]} <= {COL_CUBE[1], COL_CUBE[5], COL_CUBE[1], COL_CUBE[1], COL_CUBE[1], COL_CUBE[1], COL_CUBE[1], COL_CUBE[1], COL_CUBE[1]};
    {cube[2][0], cube[2][1], cube[2][2], cube[2][3], cube[2][4], cube[2][5], cube[2][6], cube[2][7], cube[2][8]} <= {COL_CUBE[2], COL_CUBE[2], COL_CUBE[2], COL_CUBE[2], COL_CUBE[2], COL_CUBE[2], COL_CUBE[3], COL_CUBE[2], COL_CUBE[2]};
    {cube[3][0], cube[3][1], cube[3][2], cube[3][3], cube[3][4], cube[3][5], cube[3][6], cube[3][7], cube[3][8]} <= {COL_CUBE[4], COL_CUBE[3], COL_CUBE[3], COL_CUBE[3], COL_CUBE[3], COL_CUBE[3], COL_CUBE[3], COL_CUBE[1], COL_CUBE[3]};
    {cube[4][0], cube[4][1], cube[4][2], cube[4][3], cube[4][4], cube[4][5], cube[4][6], cube[4][7], cube[4][8]} <= {COL_CUBE[4], COL_CUBE[2], COL_CUBE[4], COL_CUBE[4], COL_CUBE[4], COL_CUBE[4], COL_CUBE[4], COL_CUBE[4], COL_CUBE[2]};
    {cube[5][0], cube[5][1], cube[5][2], cube[5][3], cube[5][4], cube[5][5], cube[5][6], cube[5][7], cube[5][8]} <= {COL_CUBE[5], COL_CUBE[5], COL_CUBE[3], COL_CUBE[5], COL_CUBE[5], COL_CUBE[5], COL_CUBE[5], COL_CUBE[5], COL_CUBE[5]};
end
*/



//==============================================================================================================================
//==============================================================================================================================


// 功能：计时器
initial begin
    flag_timer = 0;
    O_timer = 0;
    O_select = 0;
end
always @(flag_timer) begin      // 高频显示 秒、分
    if (flag_timer == 2'd0) begin
        O_select <= 8'b11111110;
        O_timer <= cbtm_s0;
    end
    else if (flag_timer == 2'd1) begin
        O_select <= 8'b11111101;
        O_timer <= cbtm_s1;
    end
    else begin
        O_select <= 8'b11111011;
        O_timer <= cbtm_m0;
    end
end




// 功能：画出魔方展开图和立体图
parameter       FLAT_LX     =   11'd650;
parameter       FLAT_UY     =   11'd430;
parameter       BLOCK_SIZ   =   11'd25;
parameter       FACE_SIZ    =   11'd75;
parameter       FACE_SIZ_2  =   11'd150;
parameter       FACE_SIZ_3  =   11'd225;
parameter       FACE_SIZ_4  =   11'd300;
parameter       EDGE_COL    =   12'h0ff;
parameter       FLAT_BGC    =   12'h000;

parameter       REAL_LX     =   11'd100;
parameter       REAL_UY     =   11'd200;
parameter       REAL_SZ     =   11'd40;
parameter       REAL_SZ_2   =   2*REAL_SZ;
parameter       REAL_SZ_3   =   3*REAL_SZ;
parameter       REAL_SZ_4   =   4*REAL_SZ;
parameter       REAL_SZ_5   =   5*REAL_SZ;
parameter       REAL_SZ_6   =   6*REAL_SZ;
parameter       REAL_SZ_7   =   7*REAL_SZ;
parameter       REAL_SZ_8   =   8*REAL_SZ;
parameter       REAL_SZ_9   =   9*REAL_SZ;
parameter       REAL_SZ_11  =   11*REAL_SZ;
parameter       REAL_SZ_13  =   13*REAL_SZ;
parameter       REAL_SZ_15  =   15*REAL_SZ;
parameter       REAL_BGC    =   12'h000;

wire [10:0]tmp_rx;
wire [10:0]tmp_ry;
wire [10:0]tmp_rxy;
assign tmp_rx   = cur_x - REAL_LX;
assign tmp_ry   = cur_y - REAL_UY;
assign tmp_rxy  = cur_x + cur_y - REAL_LX - REAL_UY;

// 重要：一定要判断 W_active_flag ，为高电平才能对 O 进行输出
always @(posedge clk_65M) begin
    if (W_active_flag) begin
        
        // 展开图
        if (cur_x >= FLAT_LX && cur_y >= FLAT_UY) begin
            // 大边框-行
            if (cur_y == (FLAT_UY) || cur_y == (FLAT_UY + FACE_SIZ_3) || cur_y == (FLAT_UY + FACE_SIZ_4)) begin
                {O_red, O_green, O_blue} <= (((cur_x >= FLAT_LX + FACE_SIZ) && (cur_x <= FLAT_LX + FACE_SIZ_2)) ? EDGE_COL : FLAT_BGC);    
            end
            else if (cur_y == (FLAT_UY + FACE_SIZ) || cur_y == (FLAT_UY + FACE_SIZ_2)) begin
                {O_red, O_green, O_blue} <= (((cur_x <= FLAT_LX + FACE_SIZ_3)) ? EDGE_COL : FLAT_BGC);
            end
            // 大边框-列
            else if (cur_x == (FLAT_LX) || cur_x == (FLAT_LX + FACE_SIZ_3)) begin
                {O_red, O_green, O_blue} <= (((cur_y >= FLAT_UY + FACE_SIZ) && (cur_y <= FLAT_UY + FACE_SIZ_2)) ? EDGE_COL : FLAT_BGC);
            end
            else if (cur_x == (FLAT_LX + FACE_SIZ) || cur_x == (FLAT_LX + FACE_SIZ_2)) begin
                {O_red, O_green, O_blue} <= (((cur_y <= FLAT_UY + FACE_SIZ_4)) ? EDGE_COL : FLAT_BGC);
            end
            // 内颜色
            else if (cur_y < FLAT_UY + FACE_SIZ) begin
                if (cur_x > FLAT_LX + FACE_SIZ && cur_x < FLAT_LX + FACE_SIZ_2) begin
                    {O_red, O_green, O_blue} <= cube[0][( (cur_x-(FLAT_LX+FACE_SIZ))/BLOCK_SIZ + (cur_y-(FLAT_UY))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= cube[0][0];
                end
                else begin
                    {O_red, O_green, O_blue} <= FLAT_BGC;
                end
            end
            else if (cur_y < FLAT_UY + FACE_SIZ_2) begin
                if (cur_x < FLAT_LX + FACE_SIZ) begin
                    {O_red, O_green, O_blue} <= cube[1][( (cur_x-(FLAT_LX))/BLOCK_SIZ + (cur_y-(FLAT_UY+FACE_SIZ))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= COL_CUBE[3];
                end
                else if (cur_x < FLAT_LX + FACE_SIZ_2) begin
                    {O_red, O_green, O_blue} <= cube[2][( (cur_x-(FLAT_LX+FACE_SIZ))/BLOCK_SIZ + (cur_y-(FLAT_UY+FACE_SIZ))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= cube[2][0];
                end
                else if (cur_x < FLAT_LX + FACE_SIZ_3) begin
                    {O_red, O_green, O_blue} <= cube[3][( (cur_x-(FLAT_LX+FACE_SIZ_2))/BLOCK_SIZ + (cur_y-(FLAT_UY+FACE_SIZ))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= cube[3][0];
                end
                else begin
                    {O_red, O_green, O_blue} <= FLAT_BGC;
                end
            end
            else if (cur_y < FLAT_UY + FACE_SIZ_3) begin
                if (cur_x > FLAT_LX + FACE_SIZ && cur_x < FLAT_LX + FACE_SIZ_2) begin
                    {O_red, O_green, O_blue} <= cube[4][( (cur_x-(FLAT_LX+FACE_SIZ))/BLOCK_SIZ + (cur_y-(FLAT_UY+FACE_SIZ_2))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= cube[4][0];
                end
                else begin
                    {O_red, O_green, O_blue} <= FLAT_BGC;
                end        
            end
            else if (cur_y < FLAT_UY + FACE_SIZ_4) begin
                if (cur_x > FLAT_LX + FACE_SIZ && cur_x < FLAT_LX + FACE_SIZ_2) begin
                    {O_red, O_green, O_blue} <= cube[5][( (cur_x-(FLAT_LX+FACE_SIZ))/BLOCK_SIZ + (cur_y-(FLAT_UY+FACE_SIZ_3))/BLOCK_SIZ*3 )];
                    //{O_red, O_green, O_blue} <= cube[5][0];
                end
                else begin
                    {O_red, O_green, O_blue} <= FLAT_BGC;
                end        
            end
            else begin
                {O_red, O_green, O_blue} <= FLAT_BGC;
            end
        end
        
        // 立体图
        else if (cur_x >= REAL_LX && cur_x <= REAL_LX+REAL_SZ_9 && cur_y >= REAL_UY && cur_y <= REAL_UY+REAL_SZ_9) begin
            if (
                ((tmp_rx == 0 || tmp_rx == REAL_SZ_2 || tmp_rx == REAL_SZ_4 || tmp_rx == REAL_SZ_6) && tmp_ry >= REAL_SZ_3) ||
                ((tmp_rx == REAL_SZ_7 || tmp_rx == REAL_SZ_8 || tmp_rx == REAL_SZ_9) && tmp_rxy >= REAL_SZ_9 && tmp_rxy <= REAL_SZ_15) ||
                ((tmp_ry == 0 || tmp_ry == REAL_SZ || tmp_ry == REAL_SZ_2 || tmp_ry == REAL_SZ_3) && tmp_rxy >= REAL_SZ_3 && tmp_rxy <= REAL_SZ_9) ||
                ((tmp_ry == REAL_SZ_5 || tmp_ry == REAL_SZ_7 || tmp_ry == REAL_SZ_9) && tmp_rx <= REAL_SZ_6) ||
                ((tmp_rxy == REAL_SZ_3 || tmp_rxy == REAL_SZ_5 || tmp_rxy == REAL_SZ_7 || tmp_rxy == REAL_SZ_9) && tmp_ry <= REAL_SZ_3) ||
                ((tmp_rxy == REAL_SZ_11 || tmp_rxy == REAL_SZ_13 || tmp_rxy == REAL_SZ_15) && tmp_rx >= REAL_SZ_6)
            ) begin
                {O_red, O_green, O_blue} <= EDGE_COL;
            end
            else if (tmp_ry < REAL_SZ_3) begin
                if (tmp_rxy < REAL_SZ_3) begin
                    {O_red, O_green, O_blue} <= REAL_BGC;
                end
                else if (tmp_rxy < REAL_SZ_9) begin
                    {O_red, O_green, O_blue} <= cube[0][ (tmp_rxy-REAL_SZ_3)/REAL_SZ_2 + (tmp_ry/REAL_SZ)*3 ];
                end
                else begin
                    {O_red, O_green, O_blue} <= cube[3][ (tmp_rx-REAL_SZ_6)/REAL_SZ + (tmp_rxy - REAL_SZ_9)/REAL_SZ_2*3 ];
                end
            end
            else begin
                if (tmp_rx < REAL_SZ_6) begin
                    {O_red, O_green, O_blue} <= cube[2][ (tmp_rx)/REAL_SZ_2 + (tmp_ry-REAL_SZ_3)/REAL_SZ_2*3 ];
                end
                else if (tmp_rxy < REAL_SZ_15) begin
                    {O_red, O_green, O_blue} <= cube[3][ (tmp_rx-REAL_SZ_6)/REAL_SZ + (tmp_rxy - REAL_SZ_9)/REAL_SZ_2*3 ];
                end
                else begin
                    {O_red, O_green, O_blue} <= REAL_BGC;
                end
            end
        end
        else begin
            {O_red, O_green, O_blue} <= 12'h000;
        end
     end
     else begin
        {O_red, O_green, O_blue} <= 12'h000;
     end
end



/*
parameter H_BAR = H_DISP / 8;
parameter V_BAR = V_DISP / 8;
always @(posedge clk_65M or negedge I_rst_n) begin
    if (I_shift_cube) begin
            if (!I_rst_n) begin
            O_red   <=  4'b0000;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;
        end
        else if (W_active_flag) begin
            if (hcnt < (H_SYNC + H_BACK + H_BAR)) begin
                O_red   <=  4'b1111;
                O_green <=  4'b0000;
                O_blue  <=  4'b0000;            
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*2)) begin
                {O_red, O_green, O_blue} <= {12'b000011110000};
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*3)) begin
                {O_red, O_green, O_blue} <= {12'b000000001111};
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*4)) begin
                {O_red, O_green, O_blue} <= {12'b111111111111};
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*5)) begin
                {O_red, O_green, O_blue} <= {12'b000000000000};
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*6)) begin
                {O_red, O_green, O_blue} <= {12'b111111110000};
            end
            else if (hcnt < (H_SYNC + H_BACK + H_BAR*7)) begin
                {O_red, O_green, O_blue} <= {12'b111100001111};
            end
            else begin
                {O_red, O_green, O_blue} <= {12'b000011111111};
            end        
        end
        else begin
            O_red   <=  4'b0000;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;
        end
    end
    else begin
        if (!I_rst_n) begin
            O_red   <=  4'b0000;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;
        end
        else if (W_active_flag) begin
            if (vcnt < (V_SYNC + V_BACK + V_BAR)) begin
                O_red   <=  4'b1111;
                O_green <=  4'b0000;
                O_blue  <=  4'b0000;            
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*2)) begin
                {O_red, O_green, O_blue} <= {12'b000011110000};
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*3)) begin
                {O_red, O_green, O_blue} <= {12'b000000001111};
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*4)) begin
                {O_red, O_green, O_blue} <= {12'b111111111111};
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*5)) begin
                {O_red, O_green, O_blue} <= {12'b000000000000};
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*6)) begin
                {O_red, O_green, O_blue} <= {12'b111111110000};
            end
            else if (vcnt < (V_SYNC + V_BACK + V_BAR*7)) begin
                {O_red, O_green, O_blue} <= {12'b111100001111};
            end
            else begin
                {O_red, O_green, O_blue} <= {12'b000011111111};
            end        
        end
        else begin
            O_red   <=  4'b0000;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;
        end 
    end
end
*/


/*
// 功能：把显示器屏幕分成 8 个纵列
parameter H_BAR = H_DISP / 8;
always @(posedge clk_65M or negedge I_rst_n) begin
    if (!I_rst_n) begin
        O_red   <=  4'b0000;
        O_green <=  4'b0000;
        O_blue  <=  4'b0000;
    end
    else if (W_active_flag) begin
        if (hcnt < (H_SYNC + H_BACK + H_BAR)) begin
            O_red   <=  4'b1111;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;            
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*2)) begin
            {O_red, O_green, O_blue} <= {12'b000011110000};
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*3)) begin
            {O_red, O_green, O_blue} <= {12'b000000001111};
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*4)) begin
            {O_red, O_green, O_blue} <= {12'b111111111111};
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*5)) begin
            {O_red, O_green, O_blue} <= {12'b000000000000};
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*6)) begin
            {O_red, O_green, O_blue} <= {12'b111111110000};
        end
        else if (hcnt < (H_SYNC + H_BACK + H_BAR*7)) begin
            {O_red, O_green, O_blue} <= {12'b111100001111};
        end
        else begin
            {O_red, O_green, O_blue} <= {12'b000011111111};
        end        
    end
    else begin
        O_red   <=  4'b0000;
        O_green <=  4'b0000;
        O_blue  <=  4'b0000;
    end
end
*/

/*
// 功能：把显示器屏幕分成 8 个横行
parameter V_BAR = V_DISP / 8;
always @(posedge clk_65M or negedge I_rst_n) begin
    if (!I_rst_n) begin
        O_red   <=  4'b0000;
        O_green <=  4'b0000;
        O_blue  <=  4'b0000;
    end
    else if (W_active_flag) begin
        if (vcnt < (V_SYNC + V_BACK + V_BAR)) begin
            O_red   <=  4'b1111;
            O_green <=  4'b0000;
            O_blue  <=  4'b0000;            
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*2)) begin
            {O_red, O_green, O_blue} <= {12'b000011110000};
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*3)) begin
            {O_red, O_green, O_blue} <= {12'b000000001111};
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*4)) begin
            {O_red, O_green, O_blue} <= {12'b111111111111};
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*5)) begin
            {O_red, O_green, O_blue} <= {12'b000000000000};
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*6)) begin
            {O_red, O_green, O_blue} <= {12'b111111110000};
        end
        else if (vcnt < (V_SYNC + V_BACK + V_BAR*7)) begin
            {O_red, O_green, O_blue} <= {12'b111100001111};
        end
        else begin
            {O_red, O_green, O_blue} <= {12'b000011111111};
        end        
    end
    else begin
        O_red   <=  4'b0000;
        O_green <=  4'b0000;
        O_blue  <=  4'b0000;
    end 
end
*/
endmodule