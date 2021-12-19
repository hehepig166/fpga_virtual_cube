#include <cstring>
#include <string>
#include "cube.h"
using namespace std;


string Simplify_str(string oprstr)
{
    //return oprstr;

    string ret;
    oprstr+=' ';
    const char* oprch_rep[4]={"","","2","'"};
    int n = oprstr.size();
    char opr[100]={0};
    int rep[100]={0};
    int p=0;
    char oprch, tmpch;
    for (int i = 0; i < n; i++) {
        oprch = oprstr[i];
        if (islower(oprch))
            oprch = toupper(oprch);
        
        else if (isalpha(oprch)) {
        	
        	int tmp;
        	tmpch=oprstr[i+1];
        	if (tmpch == '\'')
            	tmp = 3;
        	else if (tmpch == '2')
        	    tmp = 2;
       		else
       			tmp = 1;
        	
            if (oprch != opr[p]) {
            	++p;
                opr[p]=oprch;
                rep[p]=tmp;
            }
            else {
                rep[p]+=tmp;
            }
        }
        
        rep[p]%=4;
        if (rep[p]==0)
        	p--;
    }
    
    for (int i=1; i<=p; i++){
    	ret+=opr[i];
    	ret+=oprch_rep[rep[i]];
    	ret+=' ';
	}
    
    return ret;
}


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

// (a,b,c,d) -> (d,a,b,c) (*rep)
inline static void _rotate_4(char &a, char &b, char &c, char &d, int rep = 1)
{
    if (rep < 0 || rep>3)
        rep = (rep % 4 + 4) % 4;

    if (rep == 0)
        return;

    if (rep == 1) {
        d ^= (c ^= (d ^= c));
        c ^= (b ^= (c ^= b));
        b ^= (a ^= (b ^= a));
    }
    else if (rep == 3) {
        a ^= (b ^= (a ^= b));
        b ^= (c ^= (b ^= c));
        c ^= (d ^= (c ^= d));
    }
    else {
        d ^= (b ^= (d ^= b));
        c ^= (a ^= (c ^= a));
    }

}






//CUBE_STATUS

void CUBE_STATUS::rotate_4(char &a, char &b, char &c, char &d, int rep)
{
    _rotate_4(a, b, c, d, rep);
}


void CUBE_STATUS::operate(const string &oprstr)
{
    int n = oprstr.size();
    int rep;
    char oprch, tmpch;
    for (int i = 0; i < n; i++) {
        oprch = oprstr[i];
        if (islower(oprch))
            oprch = toupper(oprch);
        rep = 1;
        if (i + 1 < n) {
            tmpch = oprstr[i + 1];
            if (tmpch == '\'')
                rep = 3;
            else if (tmpch == '2')
                rep = 2;
        }

        if (oprch == 'U')
            U(rep);
        else if (oprch == 'D')
            D(rep);
        else if (oprch == 'L')
            L(rep);
        else if (oprch == 'R')
            R(rep);
        else if (oprch == 'F')
            F(rep);
        else if (oprch == 'B')
            B(rep);
    }
}


#define ROTATE_4(f,a,b,c,d,r) _rotate_4(f[a],f[b],f[c],f[d],r)

void CUBE_STATUS::F(int rep)
{
    ROTATE_4(color_E, 8, 9, 10, 11, rep);
    ROTATE_4(color_E, 2, 15, 16, 5, rep);

    ROTATE_4(color_C, 8, 9, 10, 11, rep);
    ROTATE_4(color_C, 3, 12, 17, 6, rep);
    ROTATE_4(color_C, 5, 2, 15, 16, rep);
}
void CUBE_STATUS::B(int rep)
{
    ROTATE_4(color_E, 20, 21, 22, 23, rep);
    ROTATE_4(color_E, 0, 7, 18, 13, rep);

    ROTATE_4(color_C, 20, 21, 22, 23, rep);
    ROTATE_4(color_C, 1, 4, 19, 14, rep);
    ROTATE_4(color_C, 13, 0, 7, 18, rep);
}
void CUBE_STATUS::L(int rep)
{
    ROTATE_4(color_E, 4, 5, 6, 7, rep);
    ROTATE_4(color_E, 3, 11, 19, 21, rep);

    ROTATE_4(color_C, 4, 5, 6, 7, rep);
    ROTATE_4(color_C, 21, 3, 11, 19, rep);
    ROTATE_4(color_C, 0, 8, 16, 22, rep);
}
void CUBE_STATUS::R(int rep)
{
    ROTATE_4(color_E, 12, 13, 14, 15, rep);
    ROTATE_4(color_E, 1, 23, 17, 9, rep);

    ROTATE_4(color_C, 12, 13, 14, 15, rep);
    ROTATE_4(color_C, 2, 20, 18, 10, rep);
    ROTATE_4(color_C, 9, 1, 23, 17, rep);
}
void CUBE_STATUS::U(int rep)
{
    ROTATE_4(color_E, 0, 1, 2, 3, rep);
    ROTATE_4(color_E, 20, 12, 8, 4, rep);

    ROTATE_4(color_C, 0, 1, 2, 3, rep);
    ROTATE_4(color_C, 21, 13, 9, 5, rep);
    ROTATE_4(color_C, 4, 20, 12, 8, rep);
}
void CUBE_STATUS::D(int rep)
{
    ROTATE_4(color_E, 16, 17, 18, 19, rep);
    ROTATE_4(color_E, 10, 14, 22, 6, rep);

    ROTATE_4(color_C, 16, 17, 18, 19, rep);
    ROTATE_4(color_C, 6, 10, 14, 22, rep);
    ROTATE_4(color_C, 11, 15, 23, 7, rep);
}




//DATA


DATA_E eL(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eL);
    ret |= (status & BIT_E(L, U)) ? BIT_E(L, F) : 0;
    ret |= (status & BIT_E(L, F)) ? BIT_E(L, D) : 0;
    ret |= (status & BIT_E(L, D)) ? BIT_E(L, B) : 0;
    ret |= (status & BIT_E(L, B)) ? BIT_E(L, U) : 0;
    return ret;
}
DATA_E eR(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eR);
    ret |= (status & BIT_E(R, U)) ? BIT_E(R, B) : 0;
    ret |= (status & BIT_E(R, B)) ? BIT_E(R, D) : 0;
    ret |= (status & BIT_E(R, D)) ? BIT_E(R, F) : 0;
    ret |= (status & BIT_E(R, F)) ? BIT_E(R, U) : 0;
    return ret;
}
DATA_E eU(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eU);
    ret |= (status & BIT_E(U, B)) ? BIT_E(U, R) : 0;
    ret |= (status & BIT_E(U, R)) ? BIT_E(U, F) : 0;
    ret |= (status & BIT_E(U, F)) ? BIT_E(U, L) : 0;
    ret |= (status & BIT_E(U, L)) ? BIT_E(U, B) : 0;
    return ret;
}
DATA_E eD(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eD);
    ret |= (status & BIT_E(D, F)) ? BIT_E(D, R) : 0;
    ret |= (status & BIT_E(D, R)) ? BIT_E(D, B) : 0;
    ret |= (status & BIT_E(D, B)) ? BIT_E(D, L) : 0;
    ret |= (status & BIT_E(D, L)) ? BIT_E(D, F) : 0;
    return ret;
}
DATA_E eF(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eF);
    ret |= (status & BIT_E(F, U)) ? BIT_E(F, R) : 0;
    ret |= (status & BIT_E(F, R)) ? BIT_E(F, D) : 0;
    ret |= (status & BIT_E(F, D)) ? BIT_E(F, L) : 0;
    ret |= (status & BIT_E(F, L)) ? BIT_E(F, U) : 0;
    return ret;
}
DATA_E eB(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eB);
    ret |= (status & BIT_E(B, U)) ? BIT_E(B, L) : 0;
    ret |= (status & BIT_E(B, L)) ? BIT_E(B, D) : 0;
    ret |= (status & BIT_E(B, D)) ? BIT_E(B, R) : 0;
    ret |= (status & BIT_E(B, R)) ? BIT_E(B, U) : 0;
    return ret;
}

DATA_E eL_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eL);
    ret |= (status & BIT_E(L, F)) ? BIT_E(L, U) : 0;
    ret |= (status & BIT_E(L, D)) ? BIT_E(L, F) : 0;
    ret |= (status & BIT_E(L, B)) ? BIT_E(L, D) : 0;
    ret |= (status & BIT_E(L, U)) ? BIT_E(L, B) : 0;
    return ret;
}
DATA_E eR_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eR);
    ret |= (status & BIT_E(R, B)) ? BIT_E(R, U) : 0;
    ret |= (status & BIT_E(R, D)) ? BIT_E(R, B) : 0;
    ret |= (status & BIT_E(R, F)) ? BIT_E(R, D) : 0;
    ret |= (status & BIT_E(R, U)) ? BIT_E(R, F) : 0;
    return ret;
}
DATA_E eU_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eU);
    ret |= (status & BIT_E(U, R)) ? BIT_E(U, B) : 0;
    ret |= (status & BIT_E(U, F)) ? BIT_E(U, R) : 0;
    ret |= (status & BIT_E(U, L)) ? BIT_E(U, F) : 0;
    ret |= (status & BIT_E(U, B)) ? BIT_E(U, L) : 0;
    return ret;
}
DATA_E eD_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eD);
    ret |= (status & BIT_E(D, R)) ? BIT_E(D, F) : 0;
    ret |= (status & BIT_E(D, B)) ? BIT_E(D, R) : 0;
    ret |= (status & BIT_E(D, L)) ? BIT_E(D, B) : 0;
    ret |= (status & BIT_E(D, F)) ? BIT_E(D, L) : 0;
    return ret;
}
DATA_E eF_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eF);
    ret |= (status & BIT_E(F, R)) ? BIT_E(F, U) : 0;
    ret |= (status & BIT_E(F, D)) ? BIT_E(F, R) : 0;
    ret |= (status & BIT_E(F, L)) ? BIT_E(F, D) : 0;
    ret |= (status & BIT_E(F, U)) ? BIT_E(F, L) : 0;
    return ret;
}
DATA_E eB_(const DATA_E &status)
{
    DATA_E ret = status & (~BITS_eB);
    ret |= (status & BIT_E(B, L)) ? BIT_E(B, U) : 0;
    ret |= (status & BIT_E(B, D)) ? BIT_E(B, L) : 0;
    ret |= (status & BIT_E(B, R)) ? BIT_E(B, D) : 0;
    ret |= (status & BIT_E(B, U)) ? BIT_E(B, R) : 0;
    return ret;
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



DATA_C cL(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cL);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(U, L, F), 1);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(D, L, F), 2);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(D, L, B), 1);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(U, L, B), 2);
    return ret;
}
DATA_C cR(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cR);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(U, R, B), 1);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(D, R, B), 2);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(D, R, F), 1);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(U, R, F), 2);
    return ret;
}
DATA_C cU(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cU);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(U, R, B), 0);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(U, R, F), 0);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(U, L, F), 0);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(U, L, B), 0);
    return ret;
}
DATA_C cD(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cD);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(D, L, F), 0);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(D, R, F), 0);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(D, R, B), 0);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(D, L, B), 0);
    return ret;
}
DATA_C cF(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cF);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(U, R, F), 1);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(D, R, F), 2);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(D, L, F), 1);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(U, L, F), 2);
    return ret;
}
DATA_C cB(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cB);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(D, L, B), 2);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(D, R, B), 1);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(U, R, B), 2);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(U, L, B), 1);
    return ret;
}


DATA_C cL_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cL);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(D, L, B), 1);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(D, L, F), 2);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(U, L, F), 1);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(U, L, B), 2);
    return ret;
}
DATA_C cR_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cR);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(D, R, F), 1);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(D, R, B), 2);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(U, R, B), 1);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(U, R, F), 2);
    return ret;
}
DATA_C cU_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cU);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(U, L, B), 0);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(U, R, B), 0);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(U, R, F), 0);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(U, L, F), 0);
    return ret;
}
DATA_C cD_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cD);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(D, L, B), 0);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(D, L, F), 0);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(D, R, F), 0);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(D, R, B), 0);
    return ret;
}
DATA_C cF_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cF);
    ret |= modify(status & BIT_C(U, L, F), BIT_C(D, L, F), 1);
    ret |= modify(status & BIT_C(D, L, F), BIT_C(D, R, F), 2);
    ret |= modify(status & BIT_C(D, R, F), BIT_C(U, R, F), 1);
    ret |= modify(status & BIT_C(U, R, F), BIT_C(U, L, F), 2);
    return ret;
}
DATA_C cB_(const DATA_C &status)
{
    DATA_C ret = status & (~BITS_cB);
    ret |= modify(status & BIT_C(U, L, B), BIT_C(U, R, B), 2);
    ret |= modify(status & BIT_C(U, R, B), BIT_C(D, R, B), 1);
    ret |= modify(status & BIT_C(D, R, B), BIT_C(D, L, B), 2);
    ret |= modify(status & BIT_C(D, L, B), BIT_C(U, L, B), 1);
    return ret;
}
