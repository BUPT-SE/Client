#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    QString getRoomNum();//得到房间号
    QString getIP();//得到主机IP
    QString getPort();//得到主机端口
public slots:
    virtual void accept();
private:
    QLineEdit* roomNumEdit;//房间号
    QLineEdit* IPEdit;//主机IP
    QLineEdit* portEdit;//主机端口
};

#endif // CONFIGDIALOG_H
