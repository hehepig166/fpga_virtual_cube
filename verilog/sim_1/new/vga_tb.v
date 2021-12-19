`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/12/07 11:26:33
// Design Name: 
// Module Name: vga_tb
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


module vga_tb();

reg clk = 0;
reg rst_n = 1;
wire [3:0] Or;
wire [3:0] Og;
wire [3:0] Ob;
wire h1;
wire h2;
wire l1;
wire l2;

vga uut(clk, rst_n, Or, Og, Ob, h1, h2, l1, l2);

initial begin
    #10;
    rst_n = 0;
    #10;
    rst_n = 1;
end

always begin
    #1;
    clk = ~clk;
end

endmodule
