#include <iostream>
#include <unistd.h>
#include "jy_imu/imu.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

using namespace std;

bool g_flag = true;
void fun(int sig){
    g_flag = false;
}

int main(int argc, char *argv[])
{
    signal(SIGINT,fun);

    IMU imu;
    imu.setAllConfig("/dev/rplidar",115200,8,1,0,0);
    if(!imu.start())return -1;


    IMU_DATA imu_data;
    while (g_flag) {
        bool flag = imu.getData(&imu_data);
        if(flag)
            printf("roll=%f,pitch=%f,yaw=%f,tempreture=%f\n",
                imu_data.roll,imu_data.pitch,imu_data.yaw,imu_data.tempreture);
        else
            printf("no date\n");
        usleep(10000);
    }
    imu.end();

    return 0;
}




