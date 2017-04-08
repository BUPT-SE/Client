#include "client.h"
#include "ui_client.h"
#include "configdialog.h"
#include <QDebug>
#include <QJsonDocument>
#include <QByteArray>

Client::Client(QWidget *parent) : QWidget(parent), ui(new Ui::Client)
{
    //成员初始化
    ui->setupUi(this);
    _attribute = new Attribute();
    _tmpTimer = new QTimer(this);
    _updown = 0;
    _socket = new QTcpSocket(this);

    //关联信号和槽函数
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(_tmpTimer, SIGNAL(timeout()), this, SLOT(autoTmpChange()));

    //主面板先不可用
    ui->controlBox->setDisabled(true);

    //构建一个配置窗口，用于配置房间号，主机IP和端口
    ConfigDialog c;
    if(c.exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted!";
        _attribute->setRoomNum(c.getRoomNum());
        _hostIP = c.getIP();
        _hostPort = c.getPort();
    }
    else
        qDebug() << "Error!";
}

Client::~Client()
{
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
        ui->targetTmpLcd->display(QString::number(_attribute->getTargetTmp()));
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
        ui->targetTmpLcd->display(QString::number(_attribute->getTargetTmp()));
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
        //按钮设置为"关机"，面板可用
        _attribute->setPower(true);
        ui->powerButton->setText(QString::fromLocal8Bit("关机"));
        ui->controlBox->setEnabled(true);

        //与主机建立连接
        _socket->connectToHost(_hostIP, _hostPort.toInt());
    }
    
    //关机
    else if(ui->powerButton->text() == QString::fromLocal8Bit("关机"))
    {
        //按钮设置为"开机"，面板不可用
        _attribute->setPower(false);
        ui->powerButton->setText(QString::fromLocal8Bit("开机"));
        ui->controlBox->setDisabled(true);
        
        //向主机发送关机消息
        _attribute->setPower(false);
        sendMessage();
    }
}

void Client::sendMessage()
{
    qDebug() << "send message!" << endl;
    //发送json格式的消息
    QJsonDocument document;
    document.setObject(_attribute->toJson());
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    _socket->write(byteArray);
}

void Client::readMessage()
{
    //接收消息
    QByteArray byteArray = _socket->readAll();
    //将消息转化成属性
    _attribute->setFromJson(byteArray);
    //更新UI
    ui->roomTmpLcd->display(QString::number(_attribute->getRoomTmp()));//室温
    ui->modeLabel->setText(QString::fromLocal8Bit("模式：")+_attribute->getMode());//模式
    if(_attribute->getIsServed())//被服务
    {
        ui->statusLabel->setText(QString::fromLocal8Bit("状态：服务"));
        _tmpTimer->stop();
    }
    else//被挂起
    {
        ui->statusLabel->setText(QString::fromLocal8Bit("状态：挂起"));
        //启动温度变化功能
        calculate(_attribute->getRoomTmp(), _attribute->getTargetTmp());
    }
    ui->KwhLabel->setText(QString::number(_attribute->getKwh()));//所消耗能量
    ui->feeLabel->setText(QString::number(_attribute->getFee()));//金额
}

void Client::autoTmpChange()
{
    if(_updown == 1)//室温升一度，发出温控请求
    {
        _attribute->incRoomTmp();
        sendMessage();
    }
    else if(_updown == -1)//室温降一度，发出温控请求
    {
        _attribute->decRoomTmp();
        sendMessage();
    }
    else//室温保持不变
        _tmpTimer->stop();
}

void Client::calculate(float roomTmp, float defRoomTmp)
{
    if(roomTmp > defRoomTmp)        //室温比缺省室温高，温度会逐渐下降
        _updown = -1;
    else if(roomTmp < defRoomTmp)   //室温比缺省室温低，温度会逐渐上升
        _updown = 1;
    else                            //室温等于缺省室温，温度不再变化
        _updown = 0;
    _tmpTimer->setInterval(1000);
    _tmpTimer->start();
}
