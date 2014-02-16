#ifndef TC_H
#define TC_H
#include <QDialog>
#include <QString>
#include <QThread>
#include <QFile>
#include "ui_termocoppia.h"
#include "serial.hpp"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
//#include<QLed>

using namespace std;

class termocoppia;

class Thread :public QThread
{
	public:
	QString temperature;
    termocoppia* mainwindow;
	Q_OBJECT
	public:
        Thread(termocoppia *parent=0);
		protected:
		void run();
	private:
		volatile bool stopped;
        serialport usb;
    private slots:
            void stop();
};



class termocoppia : public QDialog, public Ui_Dialog
{
    Q_OBJECT
    bool isrecording;
    time_t rawtime;
    ofstream logfile;
    string* filename;
    QTimer *tic;
    QTimer *logtimer;
    Thread *reader;
public:
    termocoppia(QDialog *parent = 0);
    void serial_port_enum(void);
private slots:
    void update(void);
    void start_recording(void);
    void stop_recording(void);
    void enableApply(void);
    void disableApply(void);
    void log(void);
    void enable_open();
};
#endif
