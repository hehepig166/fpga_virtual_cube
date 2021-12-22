#include <cstdio>





void fun(char *S, int n)
{
    int a=0,b=0;
    for (int i=0; i<n; i++) {
        if (S[i]>='A' && S[i]<='Z') {
            if (i && S[i-1]!=' ') {
                printf("%d%d ",a,b);
                a=0;
            }
            switch(S[i]) {
                case 'U':   b=0;    break;
                case 'D':   b=6;    break;
                case 'L':   b=1;    break;
                case 'R':   b=2;    break;
                case 'F':   b=3;    break;
                case 'B':   b=4;    break;
                case 'M':   b=5;    break;
                case 'E':   b=7;    break;
            }
        }
        else if (S[i]=='\'') {
            a=2;
        }
        else if (S[i]=='2') {
            a=1;
        }
        else if (i && S[i-1]!=' ') {
            printf("%d%d ",a,b);
            a=0;
        }
    }
    printf("00");
    puts("\n");
}


int main()
{
    char Str[1000];
    char ch;
    int n;
    while (1) {
        printf("> ");
        for (ch=getchar(),n=0; ch!='\n'; n++, ch=getchar())
            Str[n]=ch;
        Str[n++]=' ';
        Str[n++]=0;
        fun(Str, n);
    }
}