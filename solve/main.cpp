#include <cstdio>
#include <iostream>
#include <ctime>
#include "cube.h"
using namespace std;


void test_g2();
void test_g3();

void test_CUBE_STATUS()
{
    puts("全色旋转测，打乱：U L F2 R D' B' U2 F ===============================\n");

    CUBE_STATUS A;
    A.show();

    A.U();
    A.L();
    A.F(2);
    A.R();
    A.D(-1);
    A.B(-1);
    A.U(2);
    A.F();

    A.show();

    puts("全色旋转测试结束=====================================================\n");
}

void test_g4();

int main()
{
    
    //test_CUBE_STATUS();

    CUBE_STATUS A;
    string scramble,tmpstr,solution;

    A.show();

    cout << "输入打乱  ：";
    //R2 D2 F L2 R2 B F' R2 B' L2 D' B' R' B D2 U2 B2 L' B F
    //U2 F2 U2 L2 B2 R' D2 L B2 L' B2 D' F' L2 D U B' L F2 U'
    //F2 R D F2 U R D' L' U2 B' D' R2 U F2 L2 U2 L2 U R2 D'
    //B L2 B D2 R2 F U2 L2 D2 R' F2 D2 B' U R D F D L2 B'
    //R' U' F R2 D2 R' B2 R' F2 R2 F2 D2 R D2 F R F2 R' D U L B' F D R' U' F
    getline(cin, scramble);
    A.operate(scramble);

	A.show();
	
    //cout << "打乱      ：L F2 L' F2 D2 R F2 D2 L F D B' D' U' F U2 L F' L2 R'" << endl;
    //A.operate("L F2 L' F2 D2 R F2 D2 L F D B' D' U' F U2 L F' L2 R'");

    clock_t time_stt;

    
    
    
    puts("\nG0->G1");
    time_stt = clock();
    tmpstr = G0_to_G1(A);
    cout << "用时 " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    A.operate(tmpstr);
    //A.show();
    solution += tmpstr;
    

    puts("\nG1->G2");
    time_stt = clock();
    tmpstr = G1_to_G2(A);
    cout << "用时 " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    A.operate(tmpstr);
    //A.show();
    solution += tmpstr;


    puts("\nG2->G3(虚假的)");
    time_stt = clock();
    tmpstr = G2_to_G3(A);
    cout << "用时 " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    A.operate(tmpstr);
    //A.show();
    solution += tmpstr;

    puts("\nG3->G4");
    time_stt = clock();
    tmpstr = G3_to_G4(A);
    cout << "用时 " << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
    A.operate(tmpstr);
    A.show();
    solution += tmpstr;

    //puts("=================================================================================");
    //puts("初步完成");
    //puts("=================================================================================");
    puts("\n");
    solution = Simplify_str(solution);
    cout << "初步解    ：" << solution << endl;
	

    getchar();
    return 0;
}
