`timescale 1ns / 1ps

// 参考 https://github.com/lllbbbyyy/FPGA-OV2640/blob/master/code/bluetooth.v


















module bluetooth(
    input                   clk,
    input                   rst,
    input                   get,
    output  reg     [7:0]   out,
    output  reg             submit
);

reg [7:0]   out_cache;

initial begin
    out <= 8'b0;
    out_cache <= 8'b0;
end

parameter   bps = 10417;        // 对应9600波特率

reg [14:0] count_1;
reg [3:0] count_2;

reg buffer_0, buffer_1, buffer_2;   // 滤波
wire buffer_en; // 检测边沿
reg add_en; // 加法使能信号

always @(count_2) begin
    submit <= (count_2 == 7);
end

always @(posedge clk) begin
    if (rst) begin
        {buffer_2, buffer_1, buffer_0} <= 3'b111;
    end
    else begin
        {buffer_2, buffer_1, buffer_0} <= {buffer_1, buffer_0, get};
    end
end

assign buffer_en = buffer_2 & ~buffer_1;

always @(posedge clk) begin
    if (rst) begin
        count_1 <= 0;
    end
    else if (add_en) begin
        if (count_1 == bps-1) begin
            count_1 <= 0;
        end
        else begin
            count_1 <= count_1+1;
        end
    end
end


always @(posedge clk) begin
    if (rst) begin
        count_2 <= 0;
    end
    else if (add_en && count_1 == bps-1) begin
        if (count_2 == 8) begin
            count_2 <= 0;
        end
        else begin
            count_2 <= count_2+1;
        end
    end
    else begin
        
    end
end


always @(posedge clk) begin
    if (rst) begin
        add_en <= 0;
    end
    else if (buffer_en) begin
        add_en <= 1;
    end
    else if (add_en && count_2 == 8 && count_1 == bps-1) begin
        add_en <= 0;
    end
    else begin
    
    end
end


always @(posedge clk) begin
    if (rst) begin
        out <= 0;
    end
    else if (add_en && count_1 == bps/2-1 && count_2 != 0) begin
        out[count_2-1] <= get;
    end
    else begin
    
    end
end

endmodule
