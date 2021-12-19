/************************************************************************
说明：

该步骤：复原魔方，使用 {U2, D2, L2, R2, F2, B2}

存储：棱块按能否互相到达成三组，角块分成两组。
    共五组，每组四个，编号0123，每个组占6个bit，代表当前编号 0\1\2 位置上的块的编号与位置编号的异或值（中层干脆0123四个都算上吧，凑够32位）
    最终状态应该是 DATA==0

    从上往下看，从左上角顺时针，三层的编号：
    00001111
    0123
    22223333


例子：
    21331302 0231 10012230
************************************************************************/

#include <iostream>

#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <bitset>

#include "cube.h"
using namespace std;

inline static int count_1(unsigned int x)
{
    int ret = 0;
    while (x && ++ret)
        x &= (x - 1);
    return ret;
}

inline static unsigned int lowbit(long long x)
{
    return (unsigned int)(x & (-x));
}

struct DATA {
    char s[20];

    DATA(const char *str)
    {
        for (int i = 0; i < 20; i++)
            s[i] = str[i] - '0';
    }
    DATA(const DATA &dat)
    {
        *this = dat;
    }
    DATA() {}

    operator unsigned int()
    {
        unsigned int ret = 0, tmp = 0;
        for (int i = 0; i < 4; i++, tmp += 2)
            ret |= (int)s[i] << tmp;
        for (int i = 4; i < 8; i++, tmp += 2)
            ret |= (int)(s[i] ^ 1) << tmp;
        for (int i = 8; i < 12; i++, tmp += 2)
            ret |= (int)(s[i] ^ (i - 8)) << tmp;
        for (int i = 12; i < 16; i++, tmp += 2)
            ret |= (int)(s[i] ^ 2) << tmp;
        return ret;
    }

    bool isclose()
    {
        return count_1(*this) <= 3;
        /*
        unsigned int d = unsigned int(*this);
        unsigned int tmp[5], cnt=0;
        tmp[0] = count_1(d & 0x3030000303);
        tmp[1] = count_1(d & 0x0303003030);
        tmp[2] = count_1(d & 0x0c0c000c0c);
        tmp[3] = count_1(d & 0xc0c000c0c0);
        tmp[4] = count_1(d & 0x0000ff0000);
        for (int i = 0; i < 5; i++)
            cnt += (tmp[i] > 0);
        return (cnt <= 2);
        */
    }

    void show()
    {
        for (int i = 0; i < 8; i++)
            cout << (int)s[i];
        cout << endl;
        for (int i = 8; i < 12; i++)
            cout << (int)s[i];
        cout << endl;
        for (int i = 12; i < 20; i++)
            cout << (int)s[i];
        cout << endl;
    }
};



inline static DATA U2(const DATA &status);
inline static DATA D2(const DATA &status);
inline static DATA L2(const DATA &status);
inline static DATA R2(const DATA &status);
inline static DATA F2(const DATA &status);
inline static DATA B2(const DATA &status);

static DATA(*opr[])(const DATA &) = { U2, D2, L2, R2, F2, B2 };
static const char *opr_str[] = { " 2U", " 2D", " 2L", " 2R", " 2F", " 2B" };


static string solve(DATA X, int flag=0)
{
    unordered_map<unsigned int, int> f;   //f[y] 代表 X 到 y 的最短路径中最后一步
    unordered_map<unsigned int, bool> vis;

    queue<DATA> q;

    DATA GG("00001111012322223333");
    DATA tmpGG = GG;

    q.push(X);
    vis[X] = 1;

    int cnttt = 0;

    puts("开始找，可能要几秒钟");

    //暴力bfs
    while (q.size()) {
        DATA u = q.front();

        //printf("%4x %d\n", u, q.size());

        
        if (u==GG||(flag && u.isclose())) {   //找到解，直接退出循环
            tmpGG = u;
            break;
        }

        
        if (++cnttt%10000==0) {
            putchar('.');
        }
        

        q.pop();

        for (int i = 0; i < 6; ++i) {
            DATA v = opr[i](u);
            if (vis.find(v)==vis.end()) {
                f[v] = i;
                vis[v] = true;
                q.push(v);
            }
        }
    }

    string ret;
    if (f.find(tmpGG) == f.end()) {
        puts("求解失败");
        return "-";
    }
    for (DATA cur = tmpGG; cur != X;) {
        int k = f[cur];
        ret += opr_str[k];
        cur = opr[k](cur);
    }
    reverse(ret.begin(), ret.end());    //倒序，因为前面加出来的是从 0 到 X 的路径
    puts("");
    return ret;
}






inline static DATA U2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[0], ret.s[4]);
    swap(ret.s[1], ret.s[5]);
    swap(ret.s[2], ret.s[6]);
    swap(ret.s[3], ret.s[7]);
    return ret;
}
inline static DATA D2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[12], ret.s[16]);
    swap(ret.s[13], ret.s[17]);
    swap(ret.s[14], ret.s[18]);
    swap(ret.s[15], ret.s[19]);
    return ret;
}
inline static DATA L2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[0], ret.s[18]);
    swap(ret.s[8], ret.s[11]);
    swap(ret.s[7], ret.s[19]);
    swap(ret.s[6], ret.s[12]);
    return ret;
}
inline static DATA R2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[2], ret.s[16]);
    swap(ret.s[3], ret.s[15]);
    swap(ret.s[4], ret.s[14]);
    swap(ret.s[9], ret.s[10]);
    return ret;
}
inline static DATA F2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[4], ret.s[18]);
    swap(ret.s[5], ret.s[17]);
    swap(ret.s[6], ret.s[16]);
    swap(ret.s[10], ret.s[11]);
    return ret;
}
inline static DATA B2(const DATA &status)
{
    DATA ret(status);
    swap(ret.s[0], ret.s[14]);
    swap(ret.s[1], ret.s[13]);
    swap(ret.s[2], ret.s[12]);
    swap(ret.s[8], ret.s[9]);
    return ret;
}







static int get_E_index(const CUBE_STATUS &CS, int BL_, int LB_)
{
    if (BL_ == CS_B && LB_ == CS_L)
        return 0;
    if (BL_ == CS_B && LB_ == CS_R)
        return 1;
    if (BL_ == CS_F && LB_ == CS_R)
        return 2;
    if (BL_ == CS_F && LB_ == CS_L)
        return 3;
    return -1;
}

static DATA to_DATA(const CUBE_STATUS &CS)
{
    DATA ret("00000000000000000000");

    int cU(CS_U), cD(CS_D), cL(CS_L), cR(CS_R), cF(CS_F), cB(CS_B);

    ret.s[0] |= (CS.C(0) == cD) ? 2 : 0;
    ret.s[0] |= (CS.C(21) == cF) ? 1 : 0;
    ret.s[4] |= (CS.C(2) == cD) ? 2 : 0;
    ret.s[4] |= (CS.C(9) == cF) ? 1 : 0;
    ret.s[14] |= (CS.C(18) == cD) ? 2 : 0;
    ret.s[14] |= (CS.C(23) == cF) ? 1 : 0;
    ret.s[18] |= (CS.C(16) == cD) ? 2 : 0;
    ret.s[18] |= (CS.C(11) == cF) ? 1 : 0;

    ret.s[2] |= (CS.C(1) == cD) ? 2 : 0;
    ret.s[2] |= (CS.C(20) == cF) ? 1 : 0;
    ret.s[6] |= (CS.C(3) == cD) ? 2 : 0;
    ret.s[6] |= (CS.C(8) == cF) ? 1 : 0;
    ret.s[12] |= (CS.C(19) == cD) ? 2 : 0;
    ret.s[12] |= (CS.C(22) == cF) ? 1 : 0;
    ret.s[16] |= (CS.C(17) == cD) ? 2 : 0;
    ret.s[16] |= (CS.C(10) == cF) ? 1 : 0;

    ret.s[1] |= (CS.E(0) == cD) ? 2 : 0;
    ret.s[1] |= (CS.E(20) == cF) ? 1 : 0;
    ret.s[5] |= (CS.E(2) == cD) ? 2 : 0;
    ret.s[5] |= (CS.E(8) == cF) ? 1 : 0;
    ret.s[17] |= (CS.E(16) == cD) ? 2 : 0;
    ret.s[17] |= (CS.E(10) == cF) ? 1 : 0;
    ret.s[13] |= (CS.E(18) == cD) ? 2 : 0;
    ret.s[13] |= (CS.E(22) == cF) ? 1 : 0;

    ret.s[3] |= (CS.E(1) == cD) ? 2 : 0;
    ret.s[3] |= (CS.E(12) == cL) ? 1 : 0;
    ret.s[7] |= (CS.E(3) == cD) ? 2 : 0;
    ret.s[7] |= (CS.E(4) == cL) ? 1 : 0;
    ret.s[15] |= (CS.E(17) == cD) ? 2 : 0;
    ret.s[15] |= (CS.E(14) == cL) ? 1 : 0;
    ret.s[19] |= (CS.E(19) == cD) ? 2 : 0;
    ret.s[19] |= (CS.E(6) == cL) ? 1 : 0;

    ret.s[8] = get_E_index(CS, CS.E(21), CS.E(7));
    ret.s[9] = get_E_index(CS, CS.E(23), CS.E(13));
    ret.s[10] = get_E_index(CS, CS.E(9), CS.E(15));
    ret.s[11] = get_E_index(CS, CS.E(11), CS.E(5));
    
    return ret;
}


string G3_to_G4(const CUBE_STATUS &CS)
{
    //DATA("21331302123010012230").show();
    //puts("");
    //string str = solve("21331302123010012230");
    //to_DATA(CS).show();
    string str = solve(to_DATA(CS));

    if (str == "-") {
        puts("重新求解一个接近复原的操作，之后可以自行setup PLL20/21 reverse（可以一眼看出）\n");
        puts("PLL20 ：R'U R U'| R' F' U'| F R U R' F| R' F' R U' R，交换{左上-右下}{左中-右中}\n");
        puts("PLL20 ：R U R' U | R U R' F' | R U R' U' | R' F | R2 U' R' U2 | R U' R'，交换{左下-右上}{左中-右中}\n");
        str = solve(to_DATA(CS), 1);
    }

    str = Simplify_str(str);

    cout << "该步      ：" << str << endl;
    //return str;
    return str;
}


void test_g4()
{
    //DATA aaa = DATA("00001111012322223333");
    //B2(aaa).show();
    //cout << solve(F2(R2(U2(F2(DATA("00001111012322223333")))))) << endl;
    cout << solve("21331302123010012230") << endl;
    //cout << solve("01001011021322223333") << endl;

/*
#define UU 0
#define DD 1
#define LL 2
#define RR 3
#define FF 4
#define BB 5

    int fff[] = { LL,BB,FF,UU,FF,UU,FF,RR,BB,RR,BB,RR,BB,DD,RR,DD,BB,DD,RR,DD,BB,UU,DD };
    DATA aaa = DATA("00001111012322223333");
    for (int i = 22; i >=0; i--)
        aaa = opr[fff[i]](aaa);
    aaa.show();
    cout << solve(aaa) << endl;

    puts("...");
    getchar();
*/
}
