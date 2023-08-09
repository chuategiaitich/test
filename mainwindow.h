#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_connect_clicked();       //Button kết nối
    void on_pushButton_disconnect_clicked();    //Button ngắt kết nối
    void on_pushButton_nhap_clicked();          //Button nhập dữ liệu
    void WriteData(const QByteArray data);      //Hàm viét dữ liẹu
    void ReadData();                            //Hàm đọc dữ liệu
//    void on_comboBox_currentTextChanged(const QString &arg1);
private:
    Ui::MainWindow *ui;
    QSerialPort *tue;
    dialog *minh;
    bool IS_Data_Recevived = false;
    QString Data_from_serialport;
};
#endif // MAINWINDOW_H
