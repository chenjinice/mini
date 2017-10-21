/*
 *
 * linux 串口通讯的类.
 * Created by Chen Jin on 17-06-05.
 *
 */

#include "serialport.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

SerialPort::SerialPort()
{
    m_fd = -1;
    m_device = NULL;
    this->setBaudrate(115200);
    this->setDatabits(8);
    this->setStopbits(1);
    this->setParity(0);
    this->setFlowcontrol(0);
}

SerialPort::~SerialPort()
{
    this->closePort();
}

void SerialPort::setAllConfig(char *device, int baudrate, int databits, int stopbits, int parity, int flow)
{
    m_device = device;
    this->setBaudrate(baudrate);
    this->setDatabits(databits);
    this->setStopbits(stopbits);
    this->setParity(parity);
    this->setFlowcontrol(flow);
}

void SerialPort::setDevice(char *device)
{
    m_device = device;
}

int SerialPort::setBaudrate(int baudrate)
{
    switch (baudrate) {
        case 4800:
            m_baudrate = B4800;
            break;
        case 9600:
            m_baudrate = B9600;
            break;
        case 19200:
            m_baudrate = B19200;
            break;
        case 38400:
            m_baudrate = B38400;
            break;
        case 57600:
            m_baudrate = B57600;
            break;
        case 115200:
            m_baudrate = B115200;
            break;
        case 230400:
            m_baudrate = B230400;
            break;
        default:
            m_baudrate = B115200;
            break;
    }
    return m_baudrate;
}

int SerialPort::setDatabits(int databits)
{
    switch (databits) {
        case 5:
            m_databits = CS5;
            break;
        case 6:
            m_databits = CS6;
            break;
        case 7:
            m_databits = CS7;
            break;
        case 8:
            m_databits = CS8;
            break;
        default:
            m_databits = CS8;
            break;
    }
    return m_databits;
}

int SerialPort::setStopbits(int stopbits)
{
    switch (stopbits) {
        case 1:
        case 2:
            m_stopbits = stopbits;
            break;
        default:
            m_stopbits = 1;
            break;
    }
    return m_stopbits;
}

//设置校验位:
//  0:无奇偶校验,1:奇校验，2:偶校验
int SerialPort::setParity(int parity)
{
    switch (parity) {
        case 0:
        case 1:
        case 2:
            m_parity = parity;
            break;
        default:
            m_parity = 0;
            break;
    }
    return m_parity;
}

//设置数据流控制模式：
//  0:不使用流控制,1:使用硬件流控制，2:使用软件流控制
int SerialPort::setFlowcontrol(int flow)
{
    switch (flow) {
        case 0:
        case 1:
        case 2:
            m_flowcontrols = flow;
            break;
        default:
            m_flowcontrols = 0 ;
            break;
    }
    return m_flowcontrols;
}

bool SerialPort::openPort()
{
    if(m_fd != -1)return true;
    m_fd = open(m_device,O_RDWR|O_NOCTTY|O_NDELAY);
    if(m_fd < 0){
        printf("SerialPort:open port %s failed \n",m_device);
        return false;
    }
    if( (fcntl(m_fd, F_SETFL, 0)) < 0 )
    {
        perror("SerialPort:Fcntl F_SETFL Error!\n");
        return false;
    }
    if(this->setOpt() !=0 ){
        return false;
    }
    return true;
}

void SerialPort::closePort()
{
    if(m_fd != -1){
        close(m_fd);
        m_fd = -1;
    }
}

bool SerialPort::isOpen()
{
    if (m_fd == -1) return false;
    else return true;
}

int SerialPort::readLine(char *buffer, int size)
{
    if (m_fd < 0) return -1;
    int ret =  read(m_fd, buffer, size);
    return ret;
}

int SerialPort::writeLine(char *buffer,int size)
{
    if(m_fd < 0 )return -1;
    size_t ret = write(m_fd, buffer,size);
    return (int)ret;
}

void SerialPort::clear()
{
    if(m_fd == -1)return;
    tcflush(m_fd,TCIFLUSH);
}

int SerialPort::setOpt()
{
    if(m_fd == -1)return -1;
    struct termios options;
    if(tcgetattr(m_fd,&options) != 0){
        printf("SerialPort:tcgetattr error\n");
        return -1;
    }

    /*此函数设置串口终端的以下这些属性，
termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
|INLCR|IGNCR|ICRNL|IXON);
termios_p->c_oflag &= ~OPOST;
termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
termios_p->c_cflag &= ~(CSIZE|PARENB) ;
termios_p->c_cflag |=CS8;
*/
    cfmakeraw(&options);
    //设置控制模式
    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;
    options.c_cflag &= ~CSIZE;
    //设置输入输出波特率
    cfsetispeed(&options,m_baudrate);
    cfsetospeed(&options,m_baudrate);
    //设置数据位
    options.c_cflag |= m_databits;
    //设置停止位
    switch (m_stopbits) {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
    }
    //设置校验位
    switch (m_parity) {
        case 0:
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 1:
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= (INPCK|ISTRIP);
            break;
        case 2:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= (INPCK|ISTRIP);
            break;
    }
    //设置数据流控制
    switch (m_flowcontrols) {
        case 0:
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1:
            options.c_cflag |= CRTSCTS;
            break;
        case 2:
            options.c_cflag |= IXON|IXOFF|IXANY;
            break;
    }

    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;
    tcflush(m_fd,TCIFLUSH);
    if(tcsetattr(m_fd,TCSANOW,&options)!=0){
        printf("SerialPort:set options error\n");
        return -1;
    }
    return 0;
}












