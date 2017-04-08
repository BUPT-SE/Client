#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>

class ConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigureDialog(QWidget *parent = 0);
    ~ConfigureDialog();
};

#endif // CONFIGUREDIALOG_H
