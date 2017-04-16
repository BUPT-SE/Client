#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTimer>
#include <QTcpSocket>
#include "attribute.h"

#define INTERVAL 2000

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_lowButton_clicked();

    void on_midButton_clicked();

    void on_highButton_clicked();

    void on_powerButton_clicked();

    void readMessage();             //接受消息

    void autoTmpChange();           //房间温度自动变化

    void shutDown();                 //关机

private:
    Ui::Client *ui;                 //ui
    Attribute* _attribute;          //属性
    QTimer* _tmpTimer;              //启动室温自动变化后，温度变化的计时器
    int _updown;                    //温度变化方向，增温1，降温-1
    QTcpSocket* _socket;            //用于和主机通信的套接字
    QString _hostIP;                //主机IP
    QString _hostPort;              //主机端口

private:
    void calculate(float roomTmp, float defRoomTmp);//根据室温和缺省室温计算_tmpCount和_updown
    void sendMessage();             //发送消息

};

#endif // CLIENT_H
