/*************************************************************************
	> File Name: mataqipan.c
	> Author：杜仑 
	> Mail: dulun@xiyoulinux.org
	> Created Time: Fri 09 Oct 2015 02:27:36 PM EDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 8


int CHESS[N][N];

int NextStep(int *x, int *y , int step)
{
    switch(step)
    {
        case 0:
        {
            if(*x + 1 <= N -1 && *y - 2 >= 0 && CHESS[*x +1][*y -2] == 0)
            {
                *x = *x + 1;
                *y = *y - 2;
                return 1;
            }
            break;
        }
        case 1:
        {
            if(*x + 1 <= N -1 && *y + 2 <= N -1 && CHESS[*x +1][*y+2] == 0)
            {
                *x = *x + 1;
                *y = *y + 2;
                return 1;
            }
            break;
        }
        case 2:
        {
            if(*x + 2 <= N -1 && *y - 1 >= 0 && CHESS[*x+2][*y -1] == 0)
            {
                *x = *x + 2;
                *y = *y - 1;
                return 1;
            }
            break;
        }
        case 3:
        {
            if(*x + 2 <= N -1 && *y + 1 <= N -1 && CHESS[*x +2][*y +1] == 0)
            {
                *x = *x + 2;
                *y = *y + 1;
                return 1;
            }
            break;
        }
        case 4:
        {
            if(*x - 1 >= 0 && *y - 2 >= 0 && CHESS[*x-1][*y -2] == 0)
            {
                *x = *x - 1;
                *y = *y - 2;
                return 1;
            }
            break;
        }
        case 5:
        {
            if(*x - 1 >= 0 && *y + 2 <= N -1 && CHESS[*x - 1][*y + 2] == 0)
            {
                *x = *x - 1;
                *y = *y + 2;
                return 1;
            }
            break;
        }
        case 6:
        {
            if(*x - 2 >= 0 && *y - 1 >= 0 && CHESS[*x-2][*y-1 ] == 0)
            {
                *x = *x - 2;
                *y = *y - 1;
                return 1;
            }
            break;
        }
        case 7:
        {
            if(*x - 2 >= 0 && *y + 1 <= N -1 && CHESS[*x -2][*y + 1] == 0)
            {
                *x = *x - 2;
                *y = *y + 1;
                return 1;
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

//深搜遍历
int Dp(int x, int y, int count)
{
    int x1 = x;
    int y1 = y;
    int flag = 0;
    int result , i;

    if(count > N * N )//出口，全部遍历
    {
        return 1;
    }
    
    for(i = 0; i < 8  ; i++ )
    {
        flag = NextStep(&x1, &y1, i);

        printf("%d\n", flag);
        if( 1  == flag)
        {
            CHESS[x1][y1] = count;
            result = Dp(x1, y1, count+1);//递归调用
             
            if(result)
            {
                return 1;
            }
            else{
                CHESS[x1][y1] = 0;
                x1 = x;
                y1 = y;
            }
        }
    }
  
    return 0;
}

void Print()
{
    int i, j;

    for(i = 0; i < N ;i++)
    {
        for(j = 0; j < N; j++)
        {
            printf("%5d", CHESS[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
}




int main()
{
    clock_t start, end;
    int i, j, result;

    for(i = 0; i < N; i++)    //初始化棋盘
    {
        for(j = 0; j < N; j++)
        {
            CHESS[i][j] = 0;
        }
    }

    printf("请输入起始ｘ:");
    scanf("%d", &i);
    printf("请输入起始y:");
    scanf("%d", &j);

    if(i < 0 || i > 7 || j < 0 || j > 7)
    {
        printf("输入错误，请重新输入\n");      
        return 0;
    }

    start = clock();
//    result = Rode(2,0, 2);
    //
    CHESS[i][j] = 1;
    result = Dp(i,j,2);

    printf("result = %d\n", result);


    end = clock();

    Print();

    

    return 0;
}
