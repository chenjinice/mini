#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "astar.h"
#include <sys/time.h>

using namespace std;

static int g_nx=2528;    //地图列数
static int g_ny=2176;    //地图行数
static int g_ob=1000;    //障碍个数

double get_time(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec+tv.tv_usec/1000000.0;
}

int main(int argc, char *argv[])
{
    vector<int> plan;
    //随机插入障碍

    fstream r_file("./aaa.pgm",ios::in);
    fstream w_file("./www.pgm",ios::out);
    unsigned char buffer[2528*2176]={0};
    r_file.getline((char*)buffer,g_ny*g_nx);
    w_file<<buffer<<endl;
    r_file.getline((char*)buffer,g_ny*g_nx);
    w_file<<buffer<<endl;
    r_file.getline((char*)buffer,g_ny*g_nx);
    w_file<<buffer<<endl;
    r_file.getline((char*)buffer,g_ny*g_nx);
    w_file<<buffer<<endl;
    r_file.getline((char*)buffer,g_ny*g_nx);


    astar star(g_nx,g_ny);
    int n = 1;
    double avg=0;
    for(int i=0;i<n;i++){
        double start_t = get_time();
        star.make_plan(buffer,aPoint(500,890),aPoint(2000,1300),plan);
        double end_t = get_time();
        avg+=end_t-start_t;
    }
    cout << "avg_time :" << avg/n << endl;


    cout <<"plan size:"<<plan.size()<<endl;
    for(int i=0;i<plan.size();i++){
        buffer[plan[i]] = 255;
    }
    for(int i=0;i<g_nx*g_ny;i++){
        unsigned char a = 0xff-buffer[i];
        w_file<<a;
    }
    w_file<<endl;

    r_file.close();
    w_file.close();

    return 0;
}






















