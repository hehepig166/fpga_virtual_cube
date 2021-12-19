/************************************************************************
说明：

该步骤：调整棱块、角块，使用 {U,D,L,R,F2,B2}，使之变为 {U,D,L2,R2,F2,B2}
    1.中层棱块归位中层（不需要精确对应，只要保证不在UD即可）
    2.调整角块色相，UD全朝UD（不需要精确对应）

第一部分，归位中层，与 G0_to_G1 一样，
    用一个 unsigned int 存 12 个棱块信息（是中层块为1，不是为0）

第二部分，角块色相，
    用一个 unsigned int 存 8 个角块信息（正确方向为 x00，顺拧一次为 0x01，顺拧两次为 0x11）

例子：
    (黄U红F)打乱：(L F2 L' F2 D2 R F2 D2 L F D B' D' U' F U2 L F' L2 R') (U' F D F B)
    DATA_E：0x066  0110 0110 0000
    DATA_C：0x713d  1011 1100 1000 1110

    (黄U红F)打乱：(L F2 L' F2 D2 R F2 D2 L F D B' D' U' F U2 L F' L2 R') (U' F D F B) (R D U L)
    DATA_C：0xc544  0010  0010  1010  0011

* **********************************************************************/
#include <iostream>

#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>


#include "cube.h"
using namespace std;

//两个标态
static const DATA_E GG_1 = 0xf00;
static const DATA_C GG_2 = 0x0;

//==========================================================================================================
// 第一步，双十字
//==========================================================================================================
static DATA_E _eF(const DATA_E &status)
{
    return eF(eF(status));
}
static DATA_E _eB(const DATA_E &status)
{
    return eB(eB(status));
}

static DATA_E(*const opr_e[12])(const DATA_E &) = { eL, eR, eU, eD, _eF, _eB, eL_, eR_, eU_, eD_, _eF, _eB };
static const char *opr_e_ch[12] = { "  L", "  R", "  U", "  D", " 2F", " 2B", " 'L", " 'R", " 'U", " 'D", " 2F", " 2B" };

static string solve_1(DATA_E X)
{
    //求 X 到 GG_1 的最短路径

    vector<int> dis(0x1000, 1000);   //dis[y] 代表 X 到 y 的最短路径，初始值为极大值(1000足够)
    vector<int> f(0x1000);   //f[y] 代表 X 到 y 的最短路径中最后一步
    vector<bool> vis(0x1000);
    priority_queue<pair<int, DATA_E> > q; //堆优化

    dis[X] = 0;
    q.push(make_pair(0, X));

    while (q.size() > 0) {
        DATA_E u = q.top().second;

        //printf("%4x %d\n", u, q.size());

        if (u == GG_1) {   //找到解，第一次肯定是最优的，直接退出循环
            break;
        }

        q.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        for (int i = 0; i < 10; ++i) {
            DATA_E v = opr_e[i](u);
            if (dis[v] > dis[u] + 1) {
                f[v] = i;
                dis[v] = dis[u] + 1;
                q.push(make_pair(-dis[v], v));
            }
        }
    }

    string ret;
    for (DATA_E cur = GG_1; cur != X;) {
        int k = f[cur];
        ret += opr_e_ch[k];
        cur = opr_e[k < 6 ? k + 6 : k - 6](cur);
    }
    reverse(ret.begin(), ret.end());    //倒序，因为前面加出来的是从 0 到 X 的路径

    return ret;
}


//==========================================================================================================
// 第二步，角块色相
//==========================================================================================================


static DATA_C(*const opr_c[12])(const DATA_C &) = { cL, cR, cF, cB, cL_, cR_, cF_, cB_, cU, cU_, cD, cD_ };
static const char *opr_c_str[12] = { "L  ", "R  ", "F  ", "B  ", "L' ", "R' ", "F' ", "B' ", "U  ", "U' ", "D  ", "D' " };

//顶面色相不对的角块数
inline int check_U(DATA_C x)
{
    return ((x & BIT_C(U, L, B)) > 0) + ((x & BIT_C(U, L, F)) > 0) + ((x & BIT_C(U, R, B)) > 0) + ((x & BIT_C(U, R, F)) > 0);
}

//底面色相不对的角块数
inline int check_D(DATA_C x)
{
    return ((x & BIT_C(D, L, B)) > 0) + ((x & BIT_C(D, L, F)) > 0) + ((x & BIT_C(D, R, B)) > 0) + ((x & BIT_C(D, R, F)) > 0);
}

static string solve_2(DATA_C X)
{
    string ret;
    while (X != GG_2) {

        //先确保上下都有色相不对的角块
        if (!check_D(X) || !check_U(X)) {
            if (!check_D(cR(cR(X))) || !check_U(cR(cR(X)))) {
                X = cF(cF(X));
                ret += "F  F  ";
            }
            else {
                X = cR(cR(X));
                ret += "R  R  ";
            }
        }

        int cnt = check_U(X) + check_D(X);
        int done = 0;

        //固定一个小模式： Dn, op1, op2, op3, op3, op2_, op1_
        //其中 _代表逆， op2 是 U/U_/D/D_，op1,op3 相对（比如，op1为L，则op3只能为R/R_），且不为 U/U_/D/D_

        int kkk = 0;

        for (int k = 0; !done && k <= 2; k++)
            for (int oo = 0, opd = 10; oo < 2; oo++, opd = 11)
                for (int op1 = 0; !done && op1 < 8; op1++)
                    for (int op3 = 0; !done && op3 < 8; op3++)
                        if (((op1 % 4) ^ (op3 % 4)) == 1)
                            for (int op2 = 8; !done && op2 < 12; op2++) {
                                int x = X;

                                //printf("%d\n", kkk++);

                                for (int i = 0; i < k; i++)
                                    x = opr_c[opd](x);
                                x = opr_c[op1](x);
                                x = opr_c[op2](x);
                                x = opr_c[op3](opr_c[op3](x));
                                x = opr_c[op2 ^ 1](x);
                                x = opr_c[(op1 + 4) % 8](x);

                                if (check_U(x) + check_D(x) < cnt) {    //消掉了
                                    done = 1;
                                    for (int i = 0; i < k; i++)
                                        ret += opr_c_str[opd];
                                    ret += opr_c_str[op1];
                                    ret += opr_c_str[op2];
                                    ret += opr_c_str[op3];
                                    ret += opr_c_str[op3];
                                    ret += opr_c_str[op2 ^ 1];
                                    ret += opr_c_str[(op1 + 4) % 8];

                                    X = x;
                                }
                            }

        //printf("%d\n", done);
        //ret += " | ";

    }

    return ret;
}



//==========================================================================================================
// 一些杂函数
//==========================================================================================================



//返回二进制中 1 的个数
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

//注意此处status应该最多只包含两个1，即在对应角块(BITS)的那两个bit
//返回当前这个块 顺拧 flag 次后的值
inline static DATA_C modify(DATA_C status, DATA_C BITS, int flag)
{
    int k = (count_1(status) + flag + 3) % 3;
    DATA_C b = lowbit(BITS);

    if (k == 0)
        return 0;
    else if (k == 1)
        return b;
    else
        return b | (b << 1);
}



//==========================================================================================================
// 与 CUBE_STATUS 对接
//==========================================================================================================


//对于一个棱，两个面都不是UD，那么他就是中层块，为 1，否则为 0
inline static DATA_E toDATA_E(const CUBE_STATUS &CS)
{
    DATA_E ret = 0;

    //U
    ret |= ((CS.E(0) >> 1) != 0 && (CS.E(20) >> 1) != 0) ? BIT_E(U, B) : 0;
    ret |= ((CS.E(1) >> 1) != 0 && (CS.E(12) >> 1) != 0) ? BIT_E(U, R) : 0;
    ret |= ((CS.E(2) >> 1) != 0 && (CS.E(8) >> 1) != 0) ? BIT_E(U, F) : 0;
    ret |= ((CS.E(3) >> 1) != 0 && (CS.E(4) >> 1) != 0) ? BIT_E(U, L) : 0;
    //D
    ret |= ((CS.E(16) >> 1) != 0 && (CS.E(10) >> 1) != 0) ? BIT_E(D, F) : 0;
    ret |= ((CS.E(17) >> 1) != 0 && (CS.E(14) >> 1) != 0) ? BIT_E(D, R) : 0;
    ret |= ((CS.E(18) >> 1) != 0 && (CS.E(22) >> 1) != 0) ? BIT_E(D, B) : 0;
    ret |= ((CS.E(19) >> 1) != 0 && (CS.E(6) >> 1) != 0) ? BIT_E(D, L) : 0;
    //中
    ret |= ((CS.E(9) >> 1) != 0 && (CS.E(15) >> 1) != 0) ? BIT_E(F, R) : 0;
    ret |= ((CS.E(11) >> 1) != 0 && (CS.E(5) >> 1) != 0) ? BIT_E(F, L) : 0;
    ret |= ((CS.E(21) >> 1) != 0 && (CS.E(7) >> 1) != 0) ? BIT_E(B, L) : 0;
    ret |= ((CS.E(23) >> 1) != 0 && (CS.E(13) >> 1) != 0) ? BIT_E(B, R) : 0;

    return ret;
}


//角，看UD在哪就行
inline static DATA_C toDATA_C(const CUBE_STATUS &CS)
{
    DATA_C ret = 0;

    ret |= ((CS.C(0) >> 1) == 0) ? 0 : modify(0, BIT_C(U, L, B), ((CS.C(4) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(1) >> 1) == 0) ? 0 : modify(0, BIT_C(U, R, B), ((CS.C(20) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(2) >> 1) == 0) ? 0 : modify(0, BIT_C(U, R, F), ((CS.C(12) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(3) >> 1) == 0) ? 0 : modify(0, BIT_C(U, L, F), ((CS.C(8) >> 1) == 0 ? 1 : 2));

    ret |= ((CS.C(16) >> 1) == 0) ? 0 : modify(0, BIT_C(D, L, F), ((CS.C(6) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(17) >> 1) == 0) ? 0 : modify(0, BIT_C(D, R, F), ((CS.C(10) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(18) >> 1) == 0) ? 0 : modify(0, BIT_C(D, R, B), ((CS.C(14) >> 1) == 0 ? 1 : 2));
    ret |= ((CS.C(19) >> 1) == 0) ? 0 : modify(0, BIT_C(D, L, B), ((CS.C(22) >> 1) == 0 ? 1 : 2));

    return ret;
}


string G1_to_G2(const CUBE_STATUS &CS)
{
    CUBE_STATUS cs = CS;

    DATA_E de;
    DATA_C dc;

    string t1,t2;
    t1=Simplify_str(solve_1(de=toDATA_E(cs)));

    cs.operate(t1);

    t2= Simplify_str(solve_2(dc=toDATA_C(cs)));

    //测试用
    cout << "双十字    ：" << t1 << endl;
    cout << "角块色相  ：" << t2 << endl;

    return t1+t2;
}


void test_g2()
{
    //cout << solve_2(cU(cR(cU(0)))) << endl;
    cout << "双十字    ：" << solve_1(0x066) << endl;
    cout << "角块色相  ：" << solve_2(0xc544) << endl;
}
