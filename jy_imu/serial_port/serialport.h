/*
 *
 * linux 串口通讯的类.
 * Created by Chen Jin on 17-06-05.
 *
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();
    void setAllConfig(char *device,int baudrate,int databits,int stopbits,int parity,int flow);
    void setDevice(char *device);
    int  setBaudrate(int baudrate);
    int  setDatabits(int databits);
    int  setStopbits(int stopbits);
    int  setParity(int parity);
    int  setFlowcontrol(int flow);
    bool isOpen();

protected:
    int  setOpt();
    bool openPort();
    void closePort();
    int  readLine(char *buffer,int size);
    int  writeLine(char *buffer, int size);
    void clear();

    int     m_fd;               //文件描述符
    char *  m_device;           //串口名
    int     m_baudrate;         //波特率
    int     m_databits;         //数据位
    int     m_stopbits;         //停止位
    int     m_parity;           //校验位
    int     m_flowcontrols;     //数据流控制模式
};

#endif // SERIALPORT_H







