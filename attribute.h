﻿#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QtNetWork>

#define DELTATMP 0.1

class Attribute
{
public:
    static const int SPD_LOW = 0;           //风速低档
    static const int SPD_MID = 1;           //风速中档
    static const int SPD_HIGH = 2;          //风速高档
    static const int MODE_COOL = 0;         //工作模式制冷
    static const int MODE_HEAT = 1;         //工作模式制热

public:
    Attribute();                            //构造函数
    ~Attribute();                           //析构函数
    QJsonObject toJson();                   //把属性转化成Json格式
    void setFromJson(QByteArray byteArray); //从Json格式改变属性值
    void incTargetTmp();                    //目标温度升一度
    void decTargetTmp();                    //目标温度降一度
    void incDeltaRoomTmp();                 //室温升一个单位值
    void decDeltaRoomTmp();                 //室温降一个单位值
    bool judgeInc();                        //判断目标温度是否到达上限
    bool judgeDec();                        //判断目标温度是否到达下限

    //各属性的get和set方法
    int getRoomNum() const;
    void setRoomNum(const int &roomNum);
    int getWindSpeed() const;
    void setWindSpeed(const int &windSpeed);
    double getRoomTmp() const;
    void setRoomTmp(const double &roomTmp);
    int getMode() const;
    void setMode(const int &mode);
    double getTargetTmp() const;
    void setTargetTmp(const double &targetTmp);
    bool getPower() const;
    void setPower(const bool &power);
    bool getIsServed() const;
    void setIsServed(const bool &isServed);
    double getKwh() const;
    double getFee() const;
    void setDefRoomTmp(const double &defRoomTmp);
    double getDefRoomTmp() const;

private:
    int _roomNum;                       //房间号
    int _windSpeed;                     //风速，低档，中档，高档
    double _roomTmp;                    //室温
    double _targetTmp;                  //目标温度
    int _mode;                          //工作模式，制冷，制热
    bool _power;                        //开关机
    bool _isServed;                     //是否正在被服务
    double _defRoomTmp;                 //缺省室温
    double _defTargetTmp;               //缺省目标温度
    double _Kwh;                        //消耗的电量
    double _fee;                        //所需支付的费用
    double _lowestTmp;                  //允许设置目标温度的最低温度
    double _highestTmp;                 //运行设置目标温度的最高温度
};

#endif // ATTRIBUTE_H
