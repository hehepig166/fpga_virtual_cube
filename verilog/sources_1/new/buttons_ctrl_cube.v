`timescale 1ns / 1ps






// 通过按钮控制虚拟魔方
module buttons_ctrl_cube(
    input                   clk,
    input                   I_UD,
    input                   I_LR,
    input                   I_FB,
    input                   I_Mz,
    input                   I_rst,
 
    input                   I_shift,
    input       [1:0]       I_num,
    
    input                   I_bluetooth_submit,
    input       [7:0]       I_mode,         // 连接蓝牙
    
    output                  O_act,
    output  reg [3:0]       O_mode,
    output      [1:0]       O_num,
    output                  O_rst,       
    
    output                  tmpp
);

parameter       M_U     =       4'b0000;
parameter       M_L     =       4'b0001;
parameter       M_F     =       4'b0011;
parameter       M_R     =       4'b0010;
parameter       M_D     =       4'b0110;
parameter       M_B     =       4'b0100;
parameter       M_M     =       4'b0101;
parameter       M_E     =       4'b0111;


wire tmp;
assign tmpp = tmp;
reg [31:0] cnt=32'b0;    // 小延时

assign O_num = I_num | I_mode[5:4];
assign O_rst = I_rst;
assign tmp = I_UD | I_LR | I_FB | I_Mz | I_bluetooth_submit;

assign O_act = (cnt == 32'd1);


always @(posedge clk) begin
    if (I_rst) begin
    
    end
    else if (tmp) begin
        cnt <= 32'd1000;
        
        if (I_UD) begin
            O_mode <= I_shift ? M_D : M_U;                        
        end
        else if (I_LR) begin
            O_mode <= I_shift ? M_L : M_R;
        end
        else if (I_FB) begin
            O_mode <= I_shift ? M_B : M_F;
        end
        else if (I_Mz) begin
            O_mode <= I_shift ? M_E : M_M;
        end
        else begin
            O_mode <= I_mode[3:0];
        end
    end
    else begin
        if (cnt != 0) begin
            cnt <= cnt - 32'b1;        
        end
        else begin
            
        end
    end
end

endmodule
