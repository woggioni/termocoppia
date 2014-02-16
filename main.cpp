#include <QApplication>
#include <QDialog>
#include <QtCore/QDebug>
#include "ui_termocoppia.h"
#include "tc.hpp"

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
termocoppia* ui = new termocoppia;

ui->show();
return app.exec();
}
