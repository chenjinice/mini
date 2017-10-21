#ifndef IMU_H
#define IMU_H

#include "../serial_port/serialport.h"
#include <pthread.h>

#define PACK_SIZE 1024

typedef struct _IMU_DATA{
    double roll;
    double pitch;
    double yaw;
    double tempreture;
}IMU_DATA;

class IMU : public SerialPort
{
public:
    IMU();
    ~IMU();
    bool start();
    void setRSW(char rswL, char rswH);
    bool getData(IMU_DATA *data);
    void end();

private:
    void append(char *buffer,int size);

    bool            m_pthread_state;
    pthread_t       m_thread;
    pthread_mutex_t m_lock;
    char            m_buffer[PACK_SIZE];
    static void*    read_thread(void *data);

};

#endif // IMU_H
