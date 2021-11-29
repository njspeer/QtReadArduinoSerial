#include "readserial.h"
#include "ui_readserial.h"
#include <QDebug>

ReadSerial::ReadSerial(QWidget *parent) : QMainWindow(parent), ui(new Ui::ReadSerial)
{

  ui->setupUi(this);

  /* delete output file if it exists */
  if(QFileInfo::exists(fpath)){QFile::remove(fpath);}

  /* creat a serial-port */
  serial = new QSerialPort(this);

  /* configure serial port*/
  configureSerialPort(serial);

  qDebug() << "check baudRate: " << serial->baudRate();

  /* connect &serial to &readyRead */
  connect(serial, &QSerialPort::readyRead, this, &ReadSerial::readSerialPort);

  /* disambiguate between QSerialPort::error(QSerialPort::SerialPortError) and QSerialPort::error() */
  auto error = static_cast <void(QSerialPort::*)(QSerialPort::SerialPortError)> (&QSerialPort::error);
  connect(serial, error, this, &ReadSerial::SerialError);
}

ReadSerial::~ReadSerial()
{
  delete ui;
  file.close();
}

/* get the Arduino-port serial portname */
QString ReadSerial::GetSerialPortName()
{
  QSerialPortInfo portinfo;
  QList<QSerialPortInfo> Vports = portinfo.availablePorts();
  int Ni = Vports.size();
  for(int i=0; i<Ni;++i)
  {
    QString portname = Vports[i].portName();
    if(portname.contains("ttyACM"))
    {
      return portname;
    }
  }
  /*else*/
  return "";
}

/* configure serial port */
void ReadSerial::configureSerialPort(QSerialPort *serial)
{

  /* get port name */
  QString pname = GetSerialPortName();
  if(pname.size() == 0)
  {
    QMessageBox::critical(this, tr("Error!"), "Could not find a serial port.");
    qApp->quit();
    return;
  }

  /* configure serial port */
  serial->setPortName(pname);
  serial->open(QIODevice::ReadWrite);
  serial->setBaudRate(QSerialPort::Baud9600);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setFlowControl(QSerialPort::NoFlowControl);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setDataTerminalReady(true);
}

/* slot: throw and error if there is an error on the serial port */
void ReadSerial::SerialError()
{
  qDebug() << "An error occured: " << serial->errorString();
  serial->close();
  qApp->quit();
  return;
}

/* slot: read serial data, write to file */
void ReadSerial::readSerialPort()
{
  /*clear the buffer one last time*/
  if(++xi==1){serial->clear(); return;}

  /* open file */
  QFile file(fpath);
  if(!file.isOpen())
  {
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
      QMessageBox::critical(this, tr("Error!"), "Could not open file.");
    }
  }

  /* check buffer size, if < xlen_min, then return */
  qint64 bufsize = 1024;
  char xbuffer[bufsize];
  int xlen_min = 4+3+2+1; /* 4items + 3commas + 2brackets + 1cr */
  qint64 xlen = serial->peek(xbuffer, bufsize);
  if(xlen < xlen_min){return;}
  qDebug() << xi;

  /* write data to file */
  char c;
  while(serial->getChar(&c)){file.write(&c);}
}

/* [Stop] pressed */
void ReadSerial::on_Stop_clicked()
{
  qApp->quit();
}
