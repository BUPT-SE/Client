#include "client.h"
#include "configdialog.h"
#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client w;
    w.show();

    return a.exec();
}
