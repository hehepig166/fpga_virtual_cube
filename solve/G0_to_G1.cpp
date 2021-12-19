/************************************************************************
说明：

该步骤：调整棱块色相，使用 {U,D,L,R,F,B}，使之变为 {U,D,L,R,F2,B2}

例子：
    (白U绿F)打乱：LLFFD BBUBB RRUUL BBBFD UUFFF LLUBB FFFLL
    (黄U红F)DATA_E：0x630  0000 1100 0110

    (黄U红F)打乱：L F2 L' F2 D2 R F2 D2 L F D B' D' U' F U2 L F' L2 R'
    (黄U红F)DATA_E：0xda2  0100 0101 1011

    打乱：F
    DATA_E：BITS_F

************************************************************************/
#include <string>
#include <algorithm>
#include <queue>

#include "cube.h"
using namespace std;

static DATA_E _F(const DATA_E &status)
{
    return eF(status) ^ BITS_eF;
}
static DATA_E _F_(const DATA_E &status)
{
    return eF_(status) ^ BITS_eF;
}
static DATA_E _B(const DATA_E &status)
{
    return eB(status) ^ BITS_eB;
}
static DATA_E _B_(const DATA_E &status)
{
    return eB_(status) ^ BITS_eB;
}

static DATA_E(*const opr[12])(const DATA_E &) = { eL, eR, eU, eD, _F, _B, eL_, eR_, eU_, eD_, _F_, _B_ };

static const char opr_ch[6] = { 'L', 'R', 'U', 'D', 'F', 'B' };

static string solve(DATA_E X)
{
    //求 X 到 0x0 的最短路径

    vector<int> dis(0x1000, 1000);   //dis[y] 代表 X 到 y 的最短路径，初始值为极大值(1000足够)
    vector<int> f(0x1000);   //f[y] 代表 X 到 y 的最短路径中最后一步
    vector<bool> vis(0x1000);
    priority_queue<pair<int, DATA_E> > q; //堆优化

    dis[X] = 0;
    q.push(make_pair(0, X));

    while (q.size() > 0) {
        DATA_E u = q.top().second;

        //printf("%4x %d\n", u, q.size());

        if (u == 0) {   //找到解，第一次肯定是最优的，直接退出循环
            break;
        }

        q.pop();
        if (vis[u])
            continue;
        vis[u] = true;
        for (int i = 0; i < 12; ++i) {
            DATA_E v = opr[i](u);
            if (dis[v] > dis[u] + 1) {
                f[v] = i;
                dis[v] = dis[u] + 1;
                q.push(make_pair(-dis[v], v));
            }
        }
    }

    string ret;
    for (DATA_E cur = 0; cur != X;) {
        if (f[cur] < 6) {
            ret += "  ";
            ret += opr_ch[f[cur]];
            cur = opr[f[cur] + 6](cur);
        }
        else {
            ret += " '";
            ret += opr_ch[f[cur] - 6];
            cur = opr[f[cur] - 6](cur);
        }
    }
    reverse(ret.begin(), ret.end());    //倒序，因为前面加出来的是从 0 到 X 的路径

    return ret;
}




//UD层：
//  RL在UD面
//  UD在非FDLR面（非UD面）
//中层：
//  RL在FB面
//  UD在LR面
static DATA_E toDATA(const CUBE_STATUS &CS)
{
    DATA_E ret = 0;

    //U
    ret |= ((CS.E(0) >> 1) == 1 || (CS.E(20) >> 1) == 0) ? BIT_E(U, B) : 0;
    ret |= ((CS.E(1) >> 1) == 1 || (CS.E(12) >> 1) == 0) ? BIT_E(U, R) : 0;
    ret |= ((CS.E(2) >> 1) == 1 || (CS.E(8) >> 1) == 0) ? BIT_E(U, F) : 0;
    ret |= ((CS.E(3) >> 1) == 1 || (CS.E(4) >> 1) == 0) ? BIT_E(U, L) : 0;
    //D
    ret |= ((CS.E(16) >> 1) == 1 || (CS.E(10) >> 1) == 0) ? BIT_E(D, F) : 0;
    ret |= ((CS.E(17) >> 1) == 1 || (CS.E(14) >> 1) == 0) ? BIT_E(D, R) : 0;
    ret |= ((CS.E(18) >> 1) == 1 || (CS.E(22) >> 1) == 0) ? BIT_E(D, B) : 0;
    ret |= ((CS.E(19) >> 1) == 1 || (CS.E(6) >> 1) == 0) ? BIT_E(D, L) : 0;
    //中
    ret |= ((CS.E(9) >> 1) == 1 || (CS.E(15) >> 1) == 0) ? BIT_E(F, R) : 0;
    ret |= ((CS.E(11) >> 1) == 1 || (CS.E(5) >> 1) == 0) ? BIT_E(F, L) : 0;
    ret |= ((CS.E(21) >> 1) == 1 || (CS.E(7) >> 1) == 0) ? BIT_E(B, L) : 0;
    ret |= ((CS.E(23) >> 1) == 1 || (CS.E(13) >> 1) == 0) ? BIT_E(B, R) : 0;

    return ret;
}


string G0_to_G1(const CUBE_STATUS &CS)
{
    string ret = solve(toDATA(CS));
    //测试用
    cout << "色相还原  ：" << ret << endl;
    return ret;
    //return solve(0xda2);
    //return solve(0x21c);
}
