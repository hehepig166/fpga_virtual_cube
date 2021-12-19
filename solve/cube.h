#ifndef hehepig_cube
#define hehepig_cube

#include <cstring>
#include <string>
#include <iostream>
using namespace std;


//========================================================================================
//魔方整体颜色信息
// 
// 
//========================================================================================

#define CS_U (0)
#define CS_D (1)
#define CS_L (2)
#define CS_R (3)
#define CS_F (4)
#define CS_B (5)

class CUBE_STATUS {
private:
    char color[6];      //当前六个面中心颜色{U,D,L,R,F,B}
    char color_E[24];   //当前六个面 棱 颜色
    char color_C[24];   //当前六个面 角 颜色

public:

    CUBE_STATUS()
    {
        color[0] = 'Y';
        color[1] = 'W';
        color[2] = 'B';
        color[3] = 'G';
        color[4] = 'R';
        color[5] = 'O';

        //char tmp[6] = { 'Y','B','R','G','W','O' };
        char tmp[6] = { CS_U, CS_L, CS_F, CS_R, CS_D, CS_B };

        for (int i = 0, k = 0; i < 6; i++)
            for (int j = 0; j < 4; j++, k++)
                color_C[k] = color_E[k] = tmp[i];
    }

    const char &E(int index) const
    {
        return color_E[index];
    }
    const char &C(int index) const
    {
        return color_C[index];
    }
    const char &center_color(int index) const
    {
        return color[index];
    }

    void rotate_4(char &a, char &b, char &c, char &d, int rep = 1);      // (a,b,c,d) -> (d,a,b,c) (*rep)

    void operate(const string &oprstr);

    void F(int rep = 1);
    void B(int rep = 1);
    void L(int rep = 1);
    void R(int rep = 1);
    void U(int rep = 1);
    void D(int rep = 1);

    void show() const
    {
        puts("------------------------------------------------");
        cout << "color{U, D, L, R, F, B}  \t";
        for (int i = 0; i < 6; i++)
            cout << color[i] << ' ';

        cout << "\n\ncolor_E            color_C\n";
        for (int i = 0,k1=0,k2=0; i < 6; i++) {

            for (int j = 0; j < 4; j++)
                cout << color[color_E[k1++]] << ' ';

            cout << "           ";

            for (int j = 0; j < 4; j++)
                cout << color[color_C[k2++]] << ' ';

            cout << endl;
        }
        puts("------------------------------------------------");
        cout << endl << endl;

    }
};



//========================================================================================
//魔方棱块BIT信息
// 
//用一个 unsigned int 存 12 个棱块的情况（0或1，具体意义自己定）
//顺序（低位到高位）：UB UR UF UL  DF DR DB DL  FR FL BL BR
//========================================================================================

typedef unsigned int DATA_E;

#define BIT_E(x,y) (BITS_e##x & BITS_e##y)  //例：BIT_E(U,R) 代表棱块 UR 对应的那个 bit

const DATA_E BITS_eF = 0x314;
const DATA_E BITS_eB = 0xc41;
const DATA_E BITS_eL = 0x688;
const DATA_E BITS_eR = 0x922;
const DATA_E BITS_eU = 0x00f;
const DATA_E BITS_eD = 0x0f0;

DATA_E eL(const DATA_E &status);
DATA_E eR(const DATA_E &status);
DATA_E eU(const DATA_E &status);
DATA_E eD(const DATA_E &status);
DATA_E eF(const DATA_E &status);
DATA_E eB(const DATA_E &status);

DATA_E eL_(const DATA_E &status);
DATA_E eR_(const DATA_E &status);
DATA_E eU_(const DATA_E &status);
DATA_E eD_(const DATA_E &status);
DATA_E eF_(const DATA_E &status);
DATA_E eB_(const DATA_E &status);


//========================================================================================
//魔方角块BIT信息
// 
//用一个 unsigned int 存 8 个角块的情况（0或1或2或3，具体意义自己定）
//顺序（低位到高位）：ULB URB  URF ULF  DLF DRF  DBR DBL
//========================================================================================
typedef unsigned int DATA_C;

#define BIT_C(x,y,z) (BITS_c##x & BITS_c##y & BITS_c##z)  //例：BIT_C(U,R,F) 代表角块 URF 对应的那两个 bit

const DATA_C BITS_cF = 0x0ff0;
const DATA_C BITS_cB = 0xf00f;
const DATA_C BITS_cL = 0xc3c3;
const DATA_C BITS_cR = 0x3c3c;
const DATA_C BITS_cU = 0x00ff;
const DATA_C BITS_cD = 0xff00;

DATA_C cL(const DATA_C &status);
DATA_C cR(const DATA_C &status);
DATA_C cU(const DATA_C &status);
DATA_C cD(const DATA_C &status);
DATA_C cF(const DATA_C &status);
DATA_C cB(const DATA_C &status);

DATA_C cL_(const DATA_C &status);
DATA_C cR_(const DATA_C &status);
DATA_C cU_(const DATA_C &status);
DATA_C cD_(const DATA_C &status);
DATA_C cF_(const DATA_C &status);
DATA_C cB_(const DATA_C &status);


//========================================================================================
//求解相关函数
//========================================================================================
string G0_to_G1(const CUBE_STATUS &CS);
string G1_to_G2(const CUBE_STATUS &CS);
string G2_to_G3(const CUBE_STATUS &CS);
string G3_to_G4(const CUBE_STATUS &CS);


string Simplify_str(string str);

#endif
