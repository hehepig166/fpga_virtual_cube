`timescale 1ns / 1ps





// 虚拟魔方模块

module cube_ctrl(
    input                       I_act,
    input           [3:0]       I_mode,
    input           [1:0]       I_num,          // 1+I_num
    input                       I_rst,
    output          [647:0]     O               // 每个色块 3 位， 0~5 有效
);


parameter       M_U     =       4'b0000;
parameter       M_L     =       4'b0001;
parameter       M_F     =       4'b0011;
parameter       M_R     =       4'b0010;
parameter       M_D     =       4'b0110;
parameter       M_B     =       4'b0100;
parameter       M_M     =       4'b0101;
parameter       M_E     =       4'b0111;

parameter COL_U = 12'hfff;
parameter COL_L = 12'hf80;
parameter COL_F = 12'h0f0;
parameter COL_R = 12'hf00;
parameter COL_D = 12'hff0;
parameter COL_B = 12'h00f;
wire [11:0] COL_CUBE[0:5] = {COL_U, COL_L, COL_F, COL_R, COL_D, COL_B};

parameter COLRS_INIT = {
    COL_B, COL_B, COL_B, COL_B, COL_B, COL_B, COL_B, COL_B, COL_B,
    COL_D, COL_D, COL_D, COL_D, COL_D, COL_D, COL_D, COL_D, COL_D,
    COL_R, COL_R, COL_R, COL_R, COL_R, COL_R, COL_R, COL_R, COL_R,
    COL_F, COL_F, COL_F, COL_F, COL_F, COL_F, COL_F, COL_F, COL_F,
    COL_L, COL_L, COL_L, COL_L, COL_L, COL_L, COL_L, COL_L, COL_L,
    COL_U, COL_U, COL_U, COL_U, COL_U, COL_U, COL_U, COL_U, COL_U        
};



reg         [647:0]     coldata = COLRS_INIT;
assign O = coldata;

always @(posedge I_rst, posedge I_act) begin
    if (I_rst) begin
        coldata <= COLRS_INIT;
    end
    else begin
        if (I_mode == M_U) begin
            // U
            case (I_num)
                2'b00   :
                    {coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36]}
                    <=
                    {coldata[623:612], coldata[635:624], coldata[647:636], coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[83:72], coldata[47:36], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84]};
                2'b01   :
                    {coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36]}
                    <=
                    {coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60]};
                2'b10   :
                    {coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36]}
                    <=
                    {coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[359:348], coldata[347:336], coldata[335:324], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36], coldata[11:0], coldata[23:12]};                    
                default :
                    {coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36]}
                    <=
                    {coldata[359:348], coldata[347:336], coldata[335:324], coldata[251:240], coldata[239:228], coldata[227:216], coldata[143:132], coldata[131:120], coldata[119:108], coldata[623:612], coldata[635:624], coldata[647:636], coldata[11:0], coldata[23:12], coldata[35:24], coldata[71:60], coldata[107:96], coldata[95:84], coldata[83:72], coldata[47:36]};
            endcase
        end
        else if (I_mode == M_L)begin
            // L
            case (I_num)
                2'b00   :
                    {coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144]}
                    <=
                    {coldata[551:540], coldata[587:576], coldata[623:612], coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[191:180], coldata[155:144], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192]};
                2'b01   :
                    {coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144]}
                    <=
                    {coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168]};
                    
                2'b10   :
                    {coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144]}
                    <=
                    {coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[11:0], coldata[47:36], coldata[83:72], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144], coldata[119:108], coldata[131:120]};
                default :
                    {coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144]}
                    <=
                    {coldata[11:0], coldata[47:36], coldata[83:72], coldata[227:216], coldata[263:252], coldata[299:288], coldata[443:432], coldata[479:468], coldata[515:504], coldata[551:540], coldata[587:576], coldata[623:612], coldata[119:108], coldata[131:120], coldata[143:132], coldata[179:168], coldata[215:204], coldata[203:192], coldata[191:180], coldata[155:144]};
            endcase
        end
        else if (I_mode == M_F) begin
            // F
            case (I_num)
                2'b00   :
                    {coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252]}
                    <=
                    {coldata[215:204], coldata[179:168], coldata[143:132], coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[299:288], coldata[263:252], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300]};
                2'b01   :
                    {coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252]}
                    <=
                    {coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276]};
                2'b10   :
                    {coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252]}
                    <=
                    {coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[83:72], coldata[95:84], coldata[107:96], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252], coldata[227:216], coldata[239:228]};
                default :
                    {coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252]}
                    <=
                    {coldata[83:72], coldata[95:84], coldata[107:96], coldata[335:324], coldata[371:360], coldata[407:396], coldata[467:456], coldata[455:444], coldata[443:432], coldata[215:204], coldata[179:168], coldata[143:132], coldata[227:216], coldata[239:228], coldata[251:240], coldata[287:276], coldata[323:312], coldata[311:300], coldata[299:288], coldata[263:252]};
            endcase
        end
        else if (I_mode == M_R) begin
            // R
            case (I_num)
                2'b00   :
                    {coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360]}
                    <=
                    {coldata[323:312], coldata[287:276], coldata[251:240], coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[407:396], coldata[371:360], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408]};
                2'b01   :
                    {coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360]}
                    <=
                    {coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384]};
                2'b10   :
                    {coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360]}
                    <=
                    {coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[107:96], coldata[71:60], coldata[35:24], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360], coldata[335:324], coldata[347:336]};
                default :
                    {coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360]}
                    <=
                    {coldata[107:96], coldata[71:60], coldata[35:24], coldata[647:636], coldata[611:600], coldata[575:564], coldata[539:528], coldata[503:492], coldata[467:456], coldata[323:312], coldata[287:276], coldata[251:240], coldata[335:324], coldata[347:336], coldata[359:348], coldata[395:384], coldata[431:420], coldata[419:408], coldata[407:396], coldata[371:360]};
            endcase
        end
        else if (I_mode == M_D) begin
            // D
            case (I_num)
                2'b00   :
                    {coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468]}
                    <=
                    {coldata[575:564], coldata[563:552], coldata[551:540], coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[515:504], coldata[479:468], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516]};
                2'b01   :
                    {coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468]}
                    <=
                    {coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492]};
                2'b10   :
                    {coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468]}
                    <=
                    {coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[191:180], coldata[203:192], coldata[215:204], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468], coldata[443:432], coldata[455:444]};
                default :
                    {coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468]}
                    <=
                    {coldata[191:180], coldata[203:192], coldata[215:204], coldata[299:288], coldata[311:300], coldata[323:312], coldata[407:396], coldata[419:408], coldata[431:420], coldata[575:564], coldata[563:552], coldata[551:540], coldata[443:432], coldata[455:444], coldata[467:456], coldata[503:492], coldata[539:528], coldata[527:516], coldata[515:504], coldata[479:468]};
            endcase
        end
        else if (I_mode == M_B) begin
            // B
            case (I_num)
                2'b00   :
                    {coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576]}
                    <=
                    {coldata[431:420], coldata[395:384], coldata[359:348], coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[623:612], coldata[587:576], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624]};
                2'b01   :
                    {coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576]}
                    <=
                    {coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600]};
                2'b10   :
                    {coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576]}
                    <=
                    {coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[35:24], coldata[23:12], coldata[11:0], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576], coldata[551:540], coldata[563:552]};
                default :
                    {coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576]}
                    <=
                    {coldata[35:24], coldata[23:12], coldata[11:0], coldata[119:108], coldata[155:144], coldata[191:180], coldata[515:504], coldata[527:516], coldata[539:528], coldata[431:420], coldata[395:384], coldata[359:348], coldata[551:540], coldata[563:552], coldata[575:564], coldata[611:600], coldata[647:636], coldata[635:624], coldata[623:612], coldata[587:576]};
            endcase
        end
        else if (I_mode == M_M) begin
            // M
            {coldata[635:624], coldata[599:588], coldata[563:552], coldata[527:516], coldata[491:480], coldata[455:444], coldata[311:300], coldata[275:264], coldata[239:228], coldata[95:84], coldata[59:48], coldata[23:12]}
                <=
            {coldata[95:84], coldata[59:48], coldata[23:12], coldata[635:624], coldata[599:588], coldata[563:552], coldata[527:516], coldata[491:480], coldata[455:444], coldata[311:300], coldata[275:264], coldata[239:228]};
        end
        else if (I_mode == M_E) begin
            // E
            {coldata[395:384], coldata[383:372], coldata[371:360], coldata[287:276], coldata[275:264], coldata[263:252], coldata[179:168], coldata[167:156], coldata[155:144], coldata[587:576], coldata[599:588], coldata[611:600]}
                <=
            {coldata[587:576], coldata[599:588], coldata[611:600], coldata[395:384], coldata[383:372], coldata[371:360], coldata[287:276], coldata[275:264], coldata[263:252], coldata[179:168], coldata[167:156], coldata[155:144]};
        end
        else begin
        
        end
    end

end

endmodule
