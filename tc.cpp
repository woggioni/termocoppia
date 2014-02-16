#include <QtGui>
#include "tc.hpp"


void bprint(void* s, short unsigned size){
    char * m;
    char c=1;
    int i;
    m=new char[size];
    c<<=7;
    memcpy(m,s,size);
    while (size--> 0)
            {
                for(i=0;i<8;i++)
                    {
                    cout << ((m[size] & c)!=0);
                    m[size]<<=1;
                    }
                cout << " ";
            }
    cout << endl;
    delete m;
}


termocoppia::termocoppia(QDialog* parent)
    : QDialog(parent)
{
	isrecording=0;
	filename = new string("output.dat");
    reader= new Thread(this);
    //reader->start();
	tic = new QTimer(this);
    logtimer = new QTimer(this);
    logtimer->setInterval(1000);
	tic->start(100);
    //portlist= new QListWidget;
    setWindowFlags(Qt::Window);

	setupUi(this);
    serial_port_enum();
	connect(tic, SIGNAL(timeout()), this, SLOT(update()));
	connect(RecordButton, SIGNAL(clicked()), this, SLOT(start_recording()));
	connect(StopButton, SIGNAL(clicked()), this, SLOT(stop_recording()));
    connect( Interval_spinBox, SIGNAL(valueChanged(int)), this, SLOT(enableApply()));
    connect( ApplyButton, SIGNAL(clicked()), this, SLOT(disableApply()));
    connect(logtimer, SIGNAL(timeout()), this, SLOT(log()));
    connect(filenameEdit, SIGNAL(textChanged()), this, SLOT(enableApply()));
    connect(openport, SIGNAL(clicked()), reader, SLOT(start()));
    connect(closeport, SIGNAL(clicked()), reader, SLOT(stop()));
    connect(portlist, SIGNAL(itemSelectionChanged()), this, SLOT(enable_open()));
}


void termocoppia::update(void)
{
	temperatureEdit->setText(reader->temperature);
	temperatureEdit->setAlignment( Qt::AlignCenter);
}


Thread::Thread(termocoppia *parent)
{
mainwindow=parent;
stopped = true;
}

void Thread::run()
{
    char buffer[10];
    unsigned char n;
    if (stopped)
    {
        stopped=false;
        QString portname = mainwindow->portlist->currentItem()->text();
        cout << qPrintable(portname) << endl;
        if (usb.init(portname.toStdString().c_str(), 9600)<0)
        {

            return;
        }
        else
        {
            mainwindow->closeport->setEnabled(1);
            mainwindow->openport->setEnabled(0);
            while (!stopped)
            {
                usb.flush();
                n=usb.read(buffer,10);
                buffer[n-1]='\0';
                //swap_endian((char*)&num,sizeof(short));
                //tmp=((short)*pointer);//1024*5;
                cout << buffer << endl;
                //bprint(pointer,2);
                temperature=buffer;
            }
        }
    }
}

void Thread::stop()
{
stopped = true;
if(usb.close()==0)
{
    mainwindow->closeport->setEnabled(0);
    mainwindow->openport->setEnabled(1);
    terminate();
}
else
{
    cout << "Error: impossible to close the port" << endl;
}
}

void termocoppia::start_recording()
{
	time(&rawtime);
    logtimer->start();
	logfile.open(filename->c_str());
	isrecording=1;
	logfile << "Start recording: " << ctime(&rawtime) << endl;
	StopButton->setEnabled(1);
	RecordButton->setEnabled(0);
}
	
void termocoppia::stop_recording()
{
	isrecording=0;
	time(&rawtime);
	logfile << "\nStop recording: " << ctime(&rawtime) << endl;
	logfile.close();
	StopButton->setEnabled(0);
	RecordButton->setEnabled(1);
}

void termocoppia::enableApply(void)
{
    ApplyButton->setEnabled(1);
}

void termocoppia::disableApply(void)
{
    logtimer->setInterval(1000*Interval_spinBox->value());
    *filename = filenameEdit->toPlainText().toStdString();
    ApplyButton->setEnabled(0);
}

void termocoppia::log(void)
{

    logfile << time(NULL) - rawtime << '\t' << qPrintable(reader->temperature) << endl;
}

void termocoppia::serial_port_enum(void)
{
    QDir dir=QDir::root();
    dir.cd("dev");
    dir.setFilter(QDir::Files | QDir::System);
    //dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    QString stringa, ref;
    ref="ttyACM";
    int i,j;
    for (i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        stringa=QString("%1").arg(fileInfo.fileName());
        for (j=0; j<6; j++)
        {
            if (stringa[j]!=ref[j])
            {
               goto jump;
            }
        }
       new QListWidgetItem("/dev/"+stringa,portlist);
       //std::cout << qPrintable(stringa) << std::endl;
       jump:
       ;
   }

    return;
}

void termocoppia::enable_open()
{
    if(!reader->isRunning())
    {
        openport->setEnabled(1);
    }
}
