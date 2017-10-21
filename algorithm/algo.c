#include "algo.h"
#include <stdlib.h>
#include <string.h>


int recursion(int n)
{
    if(n<=0)
        return 0;
    else if(n == 1)
        return 1;
    else
        return recursion(n-1)*n;
}


int recursion_tail(int n, int a)
{
    if(n<=0)
        return 0;
    else if(n == 1)
        return a;
    else
        return recursion_tail(n-1,n*a);
}

double r_tail(int n, double a)
{
    if(n<=0)
        return 0;
    else if (n==1)
        return a;
    else
        return r_tail(n-1,(double)1/n+a);
}

//选择排序
int sesort(int *array, int size)
{
    int i ,j;
    for(i=0;i<size;i++){
        int mm = i;
        for(j=i+1;j<size;j++){
            if(array[mm]>array[j])mm = j;
        }
        if(mm!=i){
            int temp = array[i];
            array[i]=array[mm];
            array[mm]=temp;
        }
    }
    return 0;
}

//插入排序
int issort(int *array, int size)
{
    int i,j;
    for(i=1;i<size;i++){
        int temp = array[i];
        for(j=i-1;j>=0;j--){
            if(array[j]>temp) array[j+1]=array[j];
            else break;
        }
        if((j+1)!=i){
            array[j+1]=temp;
        }
    }
    return 0;
}


//快速排序
int qksort(int *array, int size,int left,int right)
{
    if(left >=right)return -1;

    int mid  = left+(random()%(right-left+1));
    int t=array[mid];
    int l=left,r=right;

    while (1) {
        while(array[l]<t)l++;
        while(array[r]>t)r--;
        if(l>=r)break;

        if((array[r] == t) && (array[l] == t)){
            int temp = array[l];
            memcpy(array+l,array+l+1,(r-l)*sizeof(int));
            array[r]=temp;
            r-=2;
            continue;
        }

        int temp = array[l];
        array[l]=array[r];
        array[r]=temp;
    }

    qksort(array,size,left,l);
    qksort(array,size,l+1,right);
    return 0;
}


//计数排序
int ctsort(int *array,int size){
    int max = 0;
    for(int i=0;i<size;i++){
        if(array[i]>max)max = array[i];
    }

    int *temp, *counts;
    counts = (int *)malloc((max+1)*sizeof(int));
    if(counts == NULL)return -1;

    temp = (int *)malloc(size*sizeof(int));
    if(temp == NULL)return -1;

    memset(counts,0,(max+1)*sizeof(int));

    for(int i=0;i<size;i++){
        counts[array[i]]++;
    }

    for(int i=1;i<max+1;i++){
        counts[i]+=counts[i-1];
    }

    for(int i=size-1;i>=0;i--){
        int value = array[i];
        temp[counts[value]-1] = value;
        counts[value]--;
    }
    memcpy(array,temp,size*sizeof(int));
    free(counts);
    free(temp);
    return 0;
}


//基数排序
int rxsort(int *array, int size)
{

}
