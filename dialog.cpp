#include "dialog.h"

dialog::dialog(QSerialPort *myDev)
{
    tue = myDev;
}


QStringList dialog::nameport()                                   //Lấy tên thiết bị kết nối
{
    QStringList devs;
    foreach (const QSerialPortInfo info,QSerialPortInfo::availablePorts()){

        tue->setPort(info);
        if(tue->open(QIODeviceBase::ReadWrite))
        {
            tue->close();
            devs << info.portName();
        }
    }
    return devs;
}


bool dialog::Connect(QString Port)                               //Set up thông số kết nối
{
    tue->setPortName(Port);
    tue->setBaudRate(QSerialPort::BaudRate::Baud9600);
    tue->setDataBits(QSerialPort::Data8);
    tue->setParity(QSerialPort::NoParity);
    tue->setStopBits(QSerialPort::OneStop);
    tue->setFlowControl(QSerialPort::NoFlowControl);
    if(tue->open(QIODeviceBase::ReadWrite)){
        return true;
    }
    else{
        return false;
    }
}


bool dialog::Disconnect()                                        //Ngắt kết nối
{
    tue->clear();
    tue->close();
    if(tue->error() == 0|| !tue->isOpen())
    {
        qDebug() << "Đã đóng cổng serial";
        return true;
    }
    else
    {
        qDebug() << "Đóng thất bại !! Error :" << tue->error();
                                                  return false;
    }
}


qint64 dialog::Write(const char *cmd)
{
    qint64 sizeWritten;
    sizeWritten = tue->write(cmd,qstrlen(cmd));
    return sizeWritten;
}


//QString dialog::Read()
//{
//    QString buf;
//    while(tue->waitForReadyRead(20))
//    {
//        buf += tue->readAll();
//    }
//    return buf;
//}


//QString dialog::Read(int bufferSize)
//{
//    char* buf = new char[bufferSize];
//    if(tue->canReadLine())
//    {
//        tue->read(buf, bufferSize);
//    }
//    QString str = QString::fromUtf8(buf, bufferSize);
//    delete[]  buf;
//    return str;
//}
