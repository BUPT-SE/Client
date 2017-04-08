#include "attribute.h"

Attribute::Attribute()
{
    _roomNum = QString::fromLocal8Bit("");
    _windSpeed = SPD_LOW;
    _roomTmp = 28;
    _targetTmp = 25;
    _mode = 0;
    _power = false;
    _isServed = false;
    _defRoomTmp = 28;
    _Kwh = 0;
    _fee = 0;
    _lowestTmp = 15;
    _highestTmp = 30;
}

Attribute::~Attribute()
{

}

QJsonObject Attribute::toJson()
{
    QJsonObject json;
    json.insert("roomNum", _roomNum);
    json.insert("windSpeed", _windSpeed);
    json.insert("roomTmp", _roomTmp);
    json.insert("targetTmp", _targetTmp);
    json.insert("power", _power);

    return json;
}

void Attribute::setFromJson(QByteArray byteArray)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        if(doucment.isObject())
        {
            QJsonObject json = doucment.object();
            /*
            for(QString str:json.keys())
            {
                qDebug() << str << endl;
            }
            */
            //得到roomTmp
            if(json.contains("roomTmp"))
            {
                QJsonValue roomTmp = json.take("roomTmp");
                _roomTmp = roomTmp.toDouble();
            }
            //得到mode
            if(json.contains("mode"))
            {
                QJsonValue mode = json.take("mode");
                _mode = mode.toInt();
            }
            //得到isServed
            if(json.contains("isServed"))
            {
                QJsonValue isServed = json.take("isServed");
                _isServed = isServed.toBool();
            }
            //得到Kwh
            if(json.contains("Kwh"))
            {
                QJsonValue Kwh = json.take("Kwh");
                _Kwh = Kwh.toDouble();
            }
            //得到fee
            if(json.contains("fee"))
            {
                QJsonValue fee = json.take("fee");
                _fee = fee.toDouble();
            }
            //得到lowestTmp
            if(json.contains("lowestTmp"))
            {
                QJsonValue lowestTmp = json.take("lowestTmp");
                _lowestTmp = lowestTmp.toDouble();
            }
            //得到highestTmp
            if(json.contains("highestTmp"))
            {
                QJsonValue highestTmp = json.take("highestTmp");
                _highestTmp = highestTmp.toDouble();
            }
        }
    }
    else
       qDebug() << jsonError.error;
}

bool Attribute::judgeInc()
{
    //目标温度到达最高温度后则不能继续升高
    if(_targetTmp >= _highestTmp)
        return false;
    return true;
}

bool Attribute::judgeDec()
{
    //目标温度到达最低温度后则不能继续降低
    if(_targetTmp <= _lowestTmp)
        return false;
    return true;
}

void Attribute::incTargetTmp()
{
    _targetTmp++;
}

void Attribute::decTargetTmp()
{
    _targetTmp--;
}

void Attribute::incRoomTmp()
{
    _roomTmp++;
}

void Attribute::decRoomTmp()
{
    _roomTmp--;
}

QString Attribute::getRoomNum() const
{
    return _roomNum;
}

void Attribute::setRoomNum(const QString &roomNum)
{
    _roomNum = roomNum;
}

int Attribute::getWindSpeed() const
{
    return _windSpeed;
}

void Attribute::setWindSpeed(const int &windSpeed)
{
    _windSpeed = windSpeed;
}

double Attribute::getRoomTmp() const
{
    return _roomTmp;
}

void Attribute::setRoomTmp(const double &roomTmp)
{
    _roomTmp = roomTmp;
}

int Attribute::getMode() const
{
    return _mode;
}

void Attribute::setMode(const int &mode)
{
    _mode = mode;
}

double Attribute::getTargetTmp() const
{
    return _targetTmp;
}

void Attribute::setTargetTmp(const double &targetTmp)
{
    _targetTmp = targetTmp;
}

bool Attribute::getPower() const
{
    return _power;
}

void Attribute::setPower(const bool &power)
{
    _power = power;
}

bool Attribute::getIsServed() const
{
    return _isServed;
}

double Attribute::getKwh() const
{
    return _Kwh;
}

double Attribute::getFee() const
{
    return _fee;
}
