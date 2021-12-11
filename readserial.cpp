#include "readserial.h"
#include "ui_readserial.h"
#include <QDebug>

#include <QRegularExpression> /* regex */
#include <QtCharts/QLineSeries>

#include <QtCharts/QValueAxis>

#include <algorithm>

QT_CHARTS_USE_NAMESPACE

/*
 * Stop here.
 * I would like to change the name of the Vbuff class
 * Or maybe make another class that incorporates Vbuff
 * Then, I need to add that to the readSerialPort function
*/

ReadSerial::ReadSerial(QString fpth, QWidget *parent)
  : QMainWindow(parent),
  ui(new Ui::ReadSerial)
{
  ui->setupUi(this);

  /* grab some memory from the heap for a few ring buffers */
  bt = new RingBuff(Npnt);
  by1 = new RingBuff(Npnt);
  by2 = new RingBuff(Npnt);

  /* create new file object; delete old file if it exists; open */
  file = new QFile(fpth, this);
  if(file->exists()){file->remove();}
  openfile(file, QIODevice::WriteOnly | QIODevice::Append);

  /* creat a serial-port */
  serial = new QSerialPort(this);

  /* configure serial port*/
  configureSerialPort(serial);

  /* connect &serial to &readyRead */
  connect(serial, &QSerialPort::readyRead, this, &ReadSerial::readSerialPort);

  /* make new QLineSeries and x- and y-axes */
  series1 = new QtCharts::QLineSeries(this);
  series2 = new QtCharts::QLineSeries(this);
  qaxisX  = new QValueAxis(this);
  qaxisY  = new QValueAxis(this);

  ui->chartview->chart()->addSeries(series1);
  ui->chartview->chart()->addSeries(series2);

  /* make x- and y-axes and add them to QChartView */
  ui->chartview->chart()->addAxis(qaxisX, Qt::AlignBottom);
  ui->chartview->chart()->addAxis(qaxisY, Qt::AlignLeft);

  /* attach axis to series */
  series1->attachAxis(qaxisX);
  series2->attachAxis(qaxisX);
  series1->attachAxis(qaxisY);
  series2->attachAxis(qaxisY);

  /* set pen color and use OpenGL */
  series1->setPen(makePen(Qt::blue, 3));
  series2->setPen(makePen(Qt::red, 3));
  series1->setUseOpenGL(true);
  series2->setUseOpenGL(true);

  /* set title */
  ui->chartview->chart()->setTitle("Arduino Random Walk");

  /* add first two points */
  series1->append(0, 1.0);
  series2->append(0, 1.0);

  /* disambiguate between QSerialPort::error(QSerialPort::SerialPortError) and QSerialPort::error() */
  auto serialerror = static_cast <void(QSerialPort::*)(QSerialPort::SerialPortError)> (&QSerialPort::error);
  connect(serial, serialerror, this, &ReadSerial::SerialError);
}

ReadSerial::~ReadSerial()
{
  delete ui;
}

/* returns a pen specified by color and width */
QPen ReadSerial::makePen(Qt::GlobalColor color, int width)
{
  QPen pen(color);
  pen.setWidth(width);
  return pen;
}

/* open file */
void ReadSerial::openfile(QFile *file, QFlags<QIODevice::OpenMode::enum_type> mode)
{
  if(!file->isOpen())
  {
    if(!file->open(mode))
    {
      QMessageBox::critical(nullptr, "Error!", "Could not open file.");
    }
  }
}

/* close file */
void ReadSerial::closefile(QFile *file)
{
  if(file->isOpen()){file->close();}
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

/* find char c in C-style char array; returns position + 1 */
int ReadSerial::findchar(char *ch, int xlen, char c)
{
  for(int i = 0; i < xlen; ++i)
  {
    if(ch[i] == c)
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

  /* check buffer size, if < xlen_min, then return */
  char xbuffer[bufsize];
  int linelength = serial->peek(xbuffer, bufsize);
  if(linelength < linemin){return;}

  int xend = findchar(xbuffer, linelength, '\n');
  if(xend == 0){return;}

  char buf[bufsize];
  int linlength = serial->readLine(buf, bufsize);

  QString line = QString::fromStdString((std::string)buf);
  QTextStream fstream(file);

  /* parse out string */
  QRegularExpression re("<(\\d+),(\\d+),(\\d+),(\\d+)>$");
  QRegularExpressionMatch m = re.match(line);
  int indx   =  m.captured(1).toInt();
  double ti  =  m.captured(2).toFloat() * 0.001;
  double dy1 = (m.captured(3).toFloat() * (y2max) - 1) * dy;
  double dy2 = (m.captured(4).toFloat() * (y2max) - 1) * dy;

  double y1 = y1last + dy1;
  double y2 = y2last + dy2;

  bt->push(ti);
  by1->push(y1);
  by2->push(y2);

  ymax = std::max(by1->max, by2->max);
  ymin = std::min(by1->min, by2->min);


  if(bt->Ni > Npnt)
  {
    series1->remove(0);
    series2->remove(0);
  }
  series1->append(ti, y1);
  series2->append(ti, y2);

  qaxisX->setRange(bt->min, bt->max + 1);
  qaxisY->setRange(ymin, ymax);

  fstream << indx << "," << linlength << "," << ti << "," << dy1 << "," << dy2 << '\n';
  y1last = y1;
  y2last = y2;
}

/* [Stop] pressed */
void ReadSerial::on_Stop_clicked(){qApp->exit();}
