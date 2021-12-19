/************************************************************************
说明：

该步骤：调整魔方，使用 {U,D,L2,R2,F2,B2}，使之变为 {U2, D2, L2, R2, F2, B2}

考虑的只有UD两个面的棱角块，且每个块只有两状态。所以用一个 uint 记录，
    顺序：分别正对U、D，左上角开始顺时针
    在正确的位置为0，错误的位置为1

    0000 1101 0100 0011
    0xc2b0

************************************************************************/

#include <iostream>
#include <cstdio>

#include <queue>
#include <algorithm>

#include "cube.h"
using namespace std;


inline static int count_1(unsigned int x)
{
    int ret = 0;
    while (x && ++ret)
        x &= (x - 1);
    return ret;
}



typedef unsigned int DATA;

static DATA U(const DATA &status);
static DATA U_(const DATA &status);
static DATA D(const DATA &status);
static DATA D_(const DATA &status);
static DATA F2(const DATA &status);
static DATA B2(const DATA &status);
static DATA L2(const DATA &status);
static DATA R2(const DATA &status);

static DATA(*opr[])(const DATA &) = { U, U_, D, D_, L2, R2, F2, B2 };
static const char *opr_str[] = { "  U", " 'U", "  D", " 'D", " 2L", " 2R", " 2F", " 2B" };

string solve(DATA X)
{
    vector<int> dis(0x10000, 1000);   //dis[y] 代表 X 到 y 的最短路径，初始值为极大值(1000足够)
    vector<int> f(0x10000);   //f[y] 代表 X 到 y 的最短路径中最后一步
    vector<bool> vis(0x10000);
    priority_queue<pair<int, DATA> > q; //堆优化

    dis[X] = 0;
    q.push(make_pair(0, X));

    while (q.size() > 0) {
        DATA u = q.top().second;

        //printf("%4x %d\n", u, q.size());

        if (u == 0) {   //找到解，第一次肯定是最优的，直接退出循环
            break;
        }

        q.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        for (int i = 0; i < 8; ++i) {
            DATA v = opr[i](u);
            if (dis[v] > dis[u] + 1) {
                f[v] = i;
                dis[v] = dis[u] + 1;
                q.push(make_pair(-dis[v] - 2 * count_1(v), v));     //A*
                //q.push(make_pair(-dis[v] , v));
            }
        }
    }

    string ret;
    for (DATA cur = 0; cur != X;) {
        int k = f[cur];
        ret += opr_str[k];
        cur = (k < 4) ? opr[k ^ 1](cur) : opr[k](cur);
    }
    reverse(ret.begin(), ret.end());    //倒序，因为前面加出来的是从 0 到 X 的路径

    return ret;
}









static DATA U(const DATA &status)
{
    DATA ret = status & 0xff00, tmp = 0x0001;
    for (int i = 0; i < 6; ++i, tmp <<= 1)
        if (status & tmp)
            ret |= (tmp << 2);

    if (status & 0x0040)
        ret |= 0x0001;

    if (status & 0x0080)
        ret |= 0x0002;

    ret ^= 0x00ff;      //U/D操作会改变状态

    return ret;
}
static DATA U_(const DATA &status)
{
    DATA ret = status & 0xff00, tmp = 0x0004;
    for (int i = 0; i < 6; ++i, tmp <<= 1)
        if (status & tmp)
            ret |= (tmp >> 2);

    if (status & 0x0001)
        ret |= 0x0040;

    if (status & 0x0002)
        ret |= 0x0080;

    ret ^= 0x00ff;

    return ret;
}
static DATA D(const DATA &status)
{
    DATA ret = status & 0x00ff, tmp = 0x0100;
    for (int i = 0; i < 6; ++i, tmp <<= 1)
        if (status & tmp)
            ret |= (tmp << 2);

    if (status & 0x4000)
        ret |= 0x0100;

    if (status & 0x8000)
        ret |= 0x0200;

    ret ^= 0xff00;      //U/D操作会改变状态

    return ret;
}
static DATA D_(const DATA &status)
{
    DATA ret = status & 0x00ff, tmp = 0x0400;
    for (int i = 0; i < 6; ++i, tmp <<= 1)
        if (status & tmp)
            ret |= (tmp >> 2);

    if (status & 0x0100)
        ret |= 0x4000;

    if (status & 0x0200)
        ret |= 0x8000;

    ret ^= 0xff00;      //U/D操作会改变状态

    return ret;
}
static DATA F2(const DATA &status)
{
    DATA ret = status;
    if (count_1(status & 0x0440) == 1)
        ret ^= 0x0440;
    if (count_1(status & 0x0220) == 1)
        ret ^= 0x0220;
    if (count_1(status & 0x0110) == 1)
        ret ^= 0x0110;
    return ret;

}
static DATA B2(const DATA &status)
{
    DATA ret = status;
    if (count_1(status & 0x1001) == 1)
        ret ^= 0x1001;
    if (count_1(status & 0x2002) == 1)
        ret ^= 0x2002;
    if (count_1(status & 0x4004) == 1)
        ret ^= 0x4004;
    return ret;
}
static DATA L2(const DATA &status)
{
    DATA ret = status;
    if (count_1(status & 0x4040) == 1)
        ret ^= 0x4040;
    if (count_1(status & 0x8080) == 1)
        ret ^= 0x8080;
    if (count_1(status & 0x0101) == 1)
        ret ^= 0x0101;
    return ret;
}
static DATA R2(const DATA &status)
{
    DATA ret = status;
    if (count_1(status & 0x1010) == 1)
        ret ^= 0x1010;
    if (count_1(status & 0x0808) == 1)
        ret ^= 0x0808;
    if (count_1(status & 0x0404) == 1)
        ret ^= 0x0404;
    return ret;
}








//==========================================================================================================
// 与 CUBE_STATUS 对接
//==========================================================================================================




//每个块看一个颜色就行了
DATA to_DATA(const CUBE_STATUS &CS)
{
    DATA ret = 0;

    ret |= ((CS.C(21) >> 1) == 2) ? 0 : 0x0001;
    ret |= ((CS.C(20) >> 1) == 2) ? 0 : 0x0004;
    ret |= ((CS.C(9) >> 1) == 2) ? 0 : 0x0010;
    ret |= ((CS.C(8) >> 1) == 2) ? 0 : 0x0040;
    ret |= ((CS.C(11) >> 1) == 2) ? 0 : 0x0100;
    ret |= ((CS.C(10) >> 1) == 2) ? 0 : 0x0400;
    ret |= ((CS.C(23) >> 1) == 2) ? 0 : 0x1000;
    ret |= ((CS.C(22) >> 1) == 2) ? 0 : 0x4000;

    ret |= ((CS.E(20) >> 1) == 2) ? 0 : 0x0002;
    ret |= ((CS.E(12) >> 1) == 1) ? 0 : 0x0008;
    ret |= ((CS.E(8) >> 1) == 2) ? 0 : 0x0020;
    ret |= ((CS.E(4) >> 1) == 1) ? 0 : 0x0080;
    ret |= ((CS.E(10) >> 1) == 2) ? 0 : 0x0200;
    ret |= ((CS.E(14) >> 1) == 1) ? 0 : 0x0800;
    ret |= ((CS.E(22) >> 1) == 2) ? 0 : 0x2000;
    ret |= ((CS.E(6) >> 1) == 1) ? 0 : 0x8000;

    return ret;
}



string G2_to_G3(const CUBE_STATUS &CS)
{
    DATA dat = to_DATA(CS);
    string ret = Simplify_str(solve(to_DATA(CS)));

    //测试用
    cout << "全相归位  ：" << ret << endl;
    return ret;

    return ret;
}



void test_g3()
{
    //printf("%x\n", U(0));
    //printf("%x\n", R2(U(0)));
    //printf("%x\n", U(R2(U(0))));
    //printf("%x\n", D(R2(D(0))));
    //printf("%x\n", D(R2(U(0))));
    //printf("%x\n", L2(D(B2(U(R2(D(F2(U(0)))))))));

    cout << "棱角相对归位：" << solve(0xc2b0) << endl;

    //getchar();
}
