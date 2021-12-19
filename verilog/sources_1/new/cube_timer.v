`timescale 1ns / 1ps


module led_driver (
    input           [3:0] In,
    output   reg    [7:0] Out
);

always @(In) begin
    case (In)
        4'd0:   Out <= 8'h03;
        4'd1:   Out <= 8'h9f;
        4'd2:   Out <= 8'h25;
        4'd3:   Out <= 8'h0d;
        4'd4:   Out <= 8'h99; 
        4'd5:   Out <= 8'h49;
        4'd6:   Out <= 8'h41;
        4'd7:   Out <= 8'h1f;
        4'd8:   Out <= 8'h01;
        default:Out <= 8'h09;
    endcase
end
endmodule


module cube_timer(
    input                       clk_1Hz,        // 一秒时钟
    input                       I_reset,        // 异步重置
    input                       I_pause,           // 异步暂停，高电平为暂停，低电平为正常计时
    output      wire    [7:0]   O_leds_s0,
    output      wire    [7:0]   O_leds_s1,
    output      wire    [7:0]   O_leds_m0
);

reg     [3:0]   cnt_s0 = 4'b0;
reg     [3:0]   cnt_s1 = 4'b0;
reg     [3:0]   cnt_m0 = 4'b0;

led_driver ld_s0(.In(cnt_s0), .Out(O_leds_s0));
led_driver ld_s1(.In(cnt_s1), .Out(O_leds_s1));
led_driver ld_m0(.In(cnt_m0), .Out(O_leds_m0));

always @(posedge clk_1Hz, posedge I_reset, posedge I_pause) begin
    if (I_reset) begin
        cnt_s0 <= 4'b0;
        cnt_s1 <= 4'b0;
        cnt_m0 <= 4'b0;
    end
    else if (I_pause) begin
    
    end
    else begin
        if (cnt_m0 == 4'd9 && cnt_s1 == 4'd5 && cnt_s0 == 4'd9) begin
            cnt_m0 <= 4'b0;
            cnt_s1 <= 4'b0;
            cnt_s0 <= 4'b0;                        
        end
        else if (cnt_s1 == 4'd5 && cnt_s0 == 4'd9) begin
            cnt_m0 <= (cnt_m0 + 4'b1);
            cnt_s1 <= 4'b0;
            cnt_s0 <= 4'b0;
        end
        else if (cnt_s0 == 4'd9) begin
            cnt_m0 <= cnt_m0;
            cnt_s1 <= (cnt_s1 + 4'b1);
            cnt_s0 <= 4'b0;
        end
        else begin
            cnt_m0 <= cnt_m0;
            cnt_s1 <= cnt_s1;
            cnt_s0 <= (cnt_s0 + 4'b1);
        end
    end
end

endmodule
