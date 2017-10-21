#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include "algo.h"

#define N 10000000

void showtime(int *array,int n,int (*fun)(int *,int));
void showtime2(int *array,int n);

int main(void)
{
    srand(time(NULL));
    static int array[N]={0};
    for(int i=0;i<N;i++){
        int temp = rand()%N;
        array[i]=temp;
//        array2[i]=temp;
    }

    showtime(array,N,ctsort);
//    showtime2(array,N);

//    for(int i=0;i<N;i++){
//        printf("%d\n",array[i]);
//    }

    return 0;
}



void showtime(int *array,int n,int (*fun)(int *,int))
{

    float avg = 0;
    int times = 5;
    for(int i=0;i<times;i++){
        struct timeval start,end;
        gettimeofday(&start,NULL);
        fun(array,n);
        gettimeofday(&end,NULL);

        float temp  = (end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        avg+=temp;
        printf("time = %f\n",temp);
    }
    printf("avg time= %f \n",avg/times);
    return;
}

void showtime2(int *array,int n)
{

    float avg = 0;
    int times = 5;
    for(int i=0;i<times;i++){
        struct timeval start,end;
        gettimeofday(&start,NULL);
        qksort(array,n,0,n-1);
        gettimeofday(&end,NULL);

        float temp  = (end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        avg+=temp;
        printf("time = %f\n",temp);
    }
    printf("avg time= %f \n",avg/times);
    return;
}
