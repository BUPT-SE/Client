#include "client.h"
#include "ui_client.h"
#include "configdialog.h"
#include <QDebug>
#include <QJsonDocument>
#include <QByteArray>
#include <QMessageBox>

Client::Client(QWidget *parent) : QWidget(parent), ui(new Ui::Client)
{
    //构建一个配置窗口，用于配置房间号，主机IP和端口
    ConfigDialog c;
    if(c.exec() == QDialog::Accepted)
    {
        //成员初始化
        ui->setupUi(this);
        _attribute = new Attribute();
        _tmpTimer = new QTimer(this);
        _updown = 0;
        _socket = new QTcpSocket(this);

        //关联信号和槽函数
        connect(_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
        connect(_socket, SIGNAL(disconnected()), this, SLOT(shutDown()));
        connect(_tmpTimer, SIGNAL(timeout()), this, SLOT(autoTmpChange()));

        //主面板先不可用
        ui->controlBox->setDisabled(true);
        _attribute->setRoomNum(c.getRoomNum());         //配置房间号
        _attribute->setWindSpeed(Attribute::SPD_MID);   //配置默认风速
        _attribute->setDefRoomTmp(c.getDefRoomTmp());   //配置缺省室温
        _attribute->setRoomTmp(c.getDefRoomTmp());      //配置室温等于缺省室温
        _hostPort = c.getPort();                        //配置主机端口
        _hostIP = c.getIP();                            //配置主机IP地址
        ui->roomNumLabel->setText(QString::fromLocal8Bit("房间号:") + QString::number(c.getRoomNum()));
        ui->roomTmpLcd->display(QString::number((int)c.getDefRoomTmp()));
        ui->midButton->setChecked(true);
    }
}

Client::~Client()
{
    _socket->disconnectFromHost();
    delete ui;
}

//点击温度+按钮
void Client::on_upButton_clicked()
{
    //如果目标温度在最高温度之下
    if(_attribute->judgeInc())
    {
        //温度+1，更新UI，发送消息
        _attribute->incTargetTmp();
        ui->targetTmpLcd->display(QString::number((int)_attribute->getTargetTmp()));
        sendMessage();
    }
}

//点击温度-按钮
void Client::on_downButton_clicked()
{
    //如果目标温度在最低温度之上
    if(_attribute->judgeDec())
    {
        //温度-1，更新UI，发送消息
        _attribute->decTargetTmp();
        ui->targetTmpLcd->display(QString::number((int)_attribute->getTargetTmp()));
        sendMessage();
    }
}

//选择低挡风速
void Client::on_lowButton_clicked()
{
    //风速变为低挡，发送消息
    _attribute->setWindSpeed(Attribute::SPD_LOW);
    sendMessage();
}

//选择中挡风速
void Client::on_midButton_clicked()
{
    //风速变为中挡，发送消息
    _attribute->setWindSpeed(Attribute::SPD_MID);
    sendMessage();
}

//选择高挡风速
void Client::on_highButton_clicked()
{
    //风速变为高挡，发送消息
    _attribute->setWindSpeed(Attribute::SPD_HIGH);
    sendMessage();
}

//点击开机/关机按钮
void Client::on_powerButton_clicked()
{
    //开机
    if(ui->powerButton->text() == QString::fromLocal8Bit("开机"))
    {
        //与主机建立连接
        _socket->connectToHost(_hostIP, _hostPort.toInt());
        //按钮设置为"关机"，面板可用
        _attribute->setPower(true);
        _attribute->setWindSpeed(Attribute::SPD_MID);
        _attribute->setIsServed(false);
        ui->powerButton->setText(QString::fromLocal8Bit("关机"));
        ui->controlBox->setEnabled(true);
        ui->midButton->setChecked(true);//默认中风速
        ui->roomTmpLcd->display(QString::number((int)_attribute->getRoomTmp()));

        //发送第一条消息
        sendMessage();
        calculate(_attribute->getRoomTmp(), _attribute->getDefRoomTmp());
    }
    //关机
    else if(ui->powerButton->text() == QString::fromLocal8Bit("关机"))
    {   
        //向主机发送关机消息
        _attribute->setPower(false);
        sendMessage();
        _socket->disconnectFromHost();
        //按钮设置为"开机"，面板不可用
        ui->powerButton->setText(QString::fromLocal8Bit("开机"));
        ui->controlBox->setDisabled(true);
        ui->statusLabel->setText(QString::fromLocal8Bit("状态："));
        //启动温度变化功能
        _attribute->setIsServed(false);
        _attribute->setTargetTmp(-1);
        calculate(_attribute->getRoomTmp(), _attribute->getDefRoomTmp());
    }
}

void Client::sendMessage()
{
    //发送json格式的消息
    QJsonDocument document;
    document.setObject(_attribute->toJson());
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    qDebug() << "send message to server!";
    qDebug() << byteArray << endl;
    _socket->write(byteArray);
}

void Client::readMessage()
{
    //接收消息
    QByteArray byteArray = _socket->readAll();
    //将消息转化成属性
    _attribute->setFromJson(byteArray);

    qDebug() << "recieve message from server!";
    qDebug() << byteArray << endl;
    //更新UI
    //室温和目标温度
    ui->roomTmpLcd->display(QString::number(qRound(_attribute->getRoomTmp())));
    ui->targetTmpLcd->display(QString::number((int)_attribute->getTargetTmp()));
    //模式
    if(_attribute->getMode() == Attribute::MODE_COOL)
        ui->modeLabel->setText(QString::fromLocal8Bit("模式：制冷"));
    else
        ui->modeLabel->setText(QString::fromLocal8Bit("模式：制热"));
    //服务状态
    if(_attribute->getIsServed())
    {
        ui->statusLabel->setText(QString::fromLocal8Bit("状态：服务"));
        _tmpTimer->stop();
    }
    else//被挂起
    {
        ui->statusLabel->setText(QString::fromLocal8Bit("状态：挂起"));
        //启动温度变化功能
        calculate(_attribute->getRoomTmp(), _attribute->getDefRoomTmp());
    }
    //所消耗能量
    ui->KwhLabel->setText(QString::fromLocal8Bit("消耗能量(Kwh):") + QString::number(_attribute->getKwh(), 10, 2));
    //金额
    ui->feeLabel->setText(QString::fromLocal8Bit("金额(元):") + QString::number(_attribute->getFee(), 10, 2));
}

void Client::autoTmpChange()
{
    qDebug() << "autoTmpChange!";
    qDebug() << "updown:" << _updown;
    //当室温和缺省室温一致以后，温度不再变化
    if(qAbs(_attribute->getRoomTmp() - _attribute->getDefRoomTmp()) <= 0.1)
    {
        _tmpTimer->stop();
        _updown = 0;
    }

    else if(_updown == 1)//室温升一单位温度
    {
        _attribute->incDeltaRoomTmp();
        ui->roomTmpLcd->display(QString::number(qRound(_attribute->getRoomTmp())));
        if(_attribute->getPower())//关机不发消息
            sendMessage();
    }
    else if(_updown == -1)//室温降一单位温度
    {
        _attribute->decDeltaRoomTmp();
        ui->roomTmpLcd->display(QString::number(qRound(_attribute->getRoomTmp())));
        if(_attribute->getPower())//关机不发消息
            sendMessage();
    }
    ui->roomTmpLcd->display(QString::number(qRound(_attribute->getRoomTmp())));
}

void Client::shutDown()
{
    _attribute->setPower(false);
    _attribute->setIsServed(false);
    //按钮设置为"开机"，面板不可用
    ui->powerButton->setText(QString::fromLocal8Bit("开机"));
    ui->controlBox->setDisabled(true);
    ui->statusLabel->setText(QString::fromLocal8Bit("状态："));
    calculate(_attribute->getRoomTmp(), _attribute->getDefRoomTmp());
}

void Client::calculate(float roomTmp, float defRoomTmp)
{
    if(roomTmp > defRoomTmp)        //室温比缺省室温高，温度会逐渐下降
        _updown = -1;
    else if(roomTmp < defRoomTmp)   //室温比缺省室温低，温度会逐渐上升
        _updown = 1;
    else                            //室温等于缺省室温，温度不再变化
        _updown = 0;

    _tmpTimer->setInterval(INTERVAL);
    _tmpTimer->start();
}

/*
void Client::displayError(QAbstractSocket::SocketError socketError)
{
     switch (socketError)
     {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
         QMessageBox::information(this, tr("Client"),
                                  tr("The host was not found. Please check the "
                                     "host name and port settings."));
         break;
     case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::information(this, tr("Client"),
                                  tr("The connection was refused by the peer. "
                                     "Make sure the server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
         break;
     default:
         QMessageBox::information(this, tr("Client"),
                                  tr("The following error occurred: %1.")
                                  .arg(_socket->errorString()));
     }
}
*/
