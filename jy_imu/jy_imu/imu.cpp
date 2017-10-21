#include "imu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

IMU::IMU()
{
    m_pthread_state = false;
    bzero(m_buffer,PACK_SIZE);
    pthread_mutex_init(&m_lock,NULL);
}

IMU::~IMU()
{
    m_pthread_state = false;
    pthread_join(m_thread,NULL);
    pthread_mutex_destroy(&m_lock);
}

/* 创建一个线程，开始读imu数据 */
bool IMU::start()
{
    if(!this->openPort())return false;
    if(m_pthread_state)return true;
    this->clear();
    int ret = pthread_create(&m_thread,NULL,read_thread,(void *)this);
    if(ret == 0){
        m_pthread_state = true;
        return true;
    }
    else{
        m_pthread_state = false;
        return false;
    }
    this->setRSW(0x08,0x00);
}

//设置回传内容
void IMU::setRSW(char rswL, char rswH)
{
    char rsw[5];
    rsw[0] = 0xff;
    rsw[1] = 0xaa;
    rsw[2] = 0x02;
    rsw[3] = rswL;
    rsw[4] = rswH;
    this->writeLine(rsw,5);
    usleep(100000);
}

//获取imu数据
bool IMU::getData(IMU_DATA *data)
{
    /*设置一个标志位变量，每一位代表imu一种数据，
     * 这一位置1代表从imu获取到了这种数据
    */
    unsigned int flags = 0;
    bzero(data,sizeof(IMU_DATA));
    int16_t value = 0;
    unsigned char temp[PACK_SIZE];
    bzero(temp,PACK_SIZE);
    pthread_mutex_lock(&m_lock);
    memcpy(temp,m_buffer,PACK_SIZE);
    pthread_mutex_unlock(&m_lock);

    for(int i=PACK_SIZE-11;i >=0;i--){
        if(temp[i] != 0x55)continue;
        unsigned char sum = 0;
        for(int j=0;j<10;j++){
            sum+=temp[i+j];
        }
        if(sum != temp[i+10])continue;
        switch (temp[i+1]) {
            case 0x50:
                if(flags&=0x01)continue;
                flags|=0x01;
                break;
            case 0x51:
                if(flags&=0x02)continue;
                flags|=0x02;
                break;
            case 0x52:
                if(flags&=0x04)continue;
                flags|=0x04;
                break;
            case 0x53:
                //角度数据
                if(flags&=0x08)continue;
                value = (temp[i+2]|(temp[i+3]<<8));
                data->roll = value*1.0/32768*180;
                value = (temp[i+4]|(temp[i+5]<<8));
                data->pitch = value*1.0/32768*180;
                value = (temp[i+6]|(temp[i+7]<<8));
                data->yaw = value*1.0/32768*180;
                value = temp[i+8]|(temp[i+9]<<8);
                data->tempreture = value*1.0/100;
                flags|=0x08;
                break;
            case 0x54:
                if(flags&=0x10)continue;
                flags|=0x10;
                break;
            case 0x55:
                if(flags&=0x20)continue;
                flags|=0x20;
                break;
            case 0x56:
                if(flags&=0x40)continue;
                flags|=0x40;
                break;
            case 0x57:
                if(flags&=0x80)continue;
                flags|=0x80;
                break;
            case 0x58:
                if(flags&=0x0100)continue;
                flags|=0x0100;
                break;
            case 0x59:
                if(flags&=0x0200)continue;
                flags|=0x0200;
                break;
            case 0x5a:
                if(flags&=0x0400)continue;
                flags|=0x0400;
                break;
            default:
                break;
        }
    }
    if(flags)return true;
    else return false;
}

void IMU::end()
{
    this->closePort();
    m_pthread_state = false;
    pthread_join(m_thread,NULL);
    pthread_mutex_destroy(&m_lock);
}

//将读到的数据从buffer最后边插入，并挤掉之前的
void IMU::append(char *buffer, int size)
{
    if(size < 0){
        pthread_mutex_lock(&m_lock);
        bzero(m_buffer,PACK_SIZE);
        pthread_mutex_unlock(&m_lock);
        return;
    }

    int move_size = PACK_SIZE-size;
    if(move_size < 0)return;
    pthread_mutex_lock(&m_lock);
    memcpy(m_buffer,m_buffer+size,move_size);
    memcpy(m_buffer+move_size,buffer,size);
    pthread_mutex_unlock(&m_lock);
}

//读imu数据的线程函数
void *IMU::read_thread(void *data)
{
    int error_count = 0;
    IMU * ptr = (IMU*)data;
    char buf[PACK_SIZE];
    int ret = -1;
    while (ptr->m_pthread_state) {
        bzero(buf,PACK_SIZE);
        ret = ptr->readLine(buf,PACK_SIZE);
//        for(int i=0;i<ret;i++){
//            unsigned char cd = buf[i] & 0xff;
//            printf("0x%x ",cd);
//        }
//        printf("\n");
        if(ret > 0){
            ptr->append(buf,ret);
            error_count = 0;
        }else {
            error_count++;
            if(error_count >100){
                ptr->append(buf,-1);
                error_count = 0;
            }
        }
        usleep(10000);
    }
}







