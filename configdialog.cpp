#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent)
{
    //三个标签和对应的输入框
    QLabel* roomNumLabel = new QLabel(QString::fromLocal8Bit("请输入房间号:"));
    QLabel* IPLabel = new QLabel(QString::fromLocal8Bit("请输入主机IP:"));
    QLabel* portLabel = new QLabel(QString::fromLocal8Bit("请输入主机端口:"));
    QLabel* defRoomTmpLabel = new QLabel(QString::fromLocal8Bit("请输入缺省室温:"));
    roomNumEdit = new QLineEdit;
    IPEdit = new QLineEdit;
    portEdit = new QLineEdit;
    defRoomTmpEdit = new QLineEdit;

    //网格布局
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(roomNumLabel, 0, 0, 1, 1);
    gridLayout->addWidget(IPLabel, 1, 0, 1, 1);
    gridLayout->addWidget(portLabel, 2, 0, 1, 1);
    gridLayout->addWidget(defRoomTmpLabel, 3, 0, 1, 1);
    gridLayout->addWidget(roomNumEdit, 0, 1, 1, 1);
    gridLayout->addWidget(IPEdit, 1, 1, 1, 1);
    gridLayout->addWidget(portEdit, 2, 1, 1, 1);
    gridLayout->addWidget(defRoomTmpEdit, 3, 1, 1, 1);

    //确定按键
    QPushButton* okButton = new QPushButton(QString::fromLocal8Bit("确定"));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(60);
    btnLayout->addWidget(okButton);
    QVBoxLayout *dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(gridLayout);
    dlgLayout->addStretch(40);
    dlgLayout->addLayout(btnLayout);
    setLayout(dlgLayout);

    //按钮和接受槽函数关联
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    //设置大小
    resize(300,200);
}

ConfigDialog::~ConfigDialog()
{

}

int ConfigDialog::getRoomNum()
{
    return this->roomNumEdit->text().toInt();
}

QString ConfigDialog::getIP()
{
    return this->IPEdit->text();
}

QString ConfigDialog::getPort()
{
    return this->portEdit->text();
}

double ConfigDialog::getDefRoomTmp()
{
    return this->defRoomTmpEdit->text().toDouble();
}

void ConfigDialog::accept()
{
    //房间号，主机IP，主机端口号，缺省室温内容都不为空时，才返回accept
    if (roomNumEdit->text() != nullptr
        && IPEdit->text() != nullptr
        && portEdit->text() != nullptr
        && defRoomTmpEdit->text() != nullptr)
    {
        QDialog::accept();
    }
}
