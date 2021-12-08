#include "readserial.h"
#include "ui_readserial.h"
#include <QDebug>

#include <QRegularExpression>

ReadSerial::ReadSerial(QString fpth, QWidget *parent)
  : QMainWindow(parent),
  ui(new Ui::ReadSerial)
{
  ui->setupUi(this);

  /* set fpath */
  fpath = fpth;

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
  auto serialerror = static_cast <void(QSerialPort::*)(QSerialPort::SerialPortError)> (&QSerialPort::error);
  connect(serial, serialerror, this, &ReadSerial::SerialError);
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

/* slot: throw an error if there is an error on the serial port */
void ReadSerial::SerialError()
{
  qDebug() << "An error occured: " << serial->errorString();
  serial->close();
  qApp->quit();
  return;
}

/* find '\n' in C-style char array */
int ReadSerial::findchar(char *Ach, int xlen)
{
  for(int i = 0; i < xlen; ++i)
  {
    if(Ach[i] == '\n')
    {
      return i+1;
    }
  }
  return 0;
}

/* slot: read serial data, write to file */
void ReadSerial::readSerialPort()
{
  /* clear the buffer one last time */
  if(++xi == 1){serial->clear(); return;}

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
  const int xlen_min = 4 + 3 + 2 + 1; /* 4items + 3',' + 2'{}' + 1'\n' */
  char xbuffer[bufsize];
  qint64 xlen = serial->peek(xbuffer, bufsize);
  if(xlen < xlen_min){return;}

  int xend = findchar(xbuffer, xlen);

  if(xend == 0){return;}

//  char line[xend+1];
//  serial->readLine(line, xend);
//  qDebug() << xi << ", " << line;
  qDebug() << "xend = " << xend;

  QString line;
  QTextStream strstream(&line);
  QTextStream fstream(&file);


  /* write data to file and stream*/
  char c;
  for(int i = 0; i < xend; ++i)
  {
    if(serial->getChar(&c))
    {
//      file.write(&c);
      strstream << c;
    }
  }

  /* parse out string */
  QRegularExpression re("<(\\d+),(\\d+),(\\d+),(\\d+)>$");
  QRegularExpressionMatch m = re.match(line);
  int indx  = m.captured(1).toInt();
  float dt  = m.captured(2).toFloat()*0.001;
  qreal dy1 = m.captured(3).toFloat()*(4.656612875245797e-10);
  qreal dy2 = m.captured(4).toFloat()*(4.656612875245797e-10);

  fstream << indx << "," << dt << "," << dy1 << "," << dy2 << '\n';
  qDebug() << indx << "," << dt << "," << dy1 << "," << dy2 << '\n';

}

/* [Stop] pressed */
void ReadSerial::on_Stop_clicked()
{
  qApp->quit();
}
