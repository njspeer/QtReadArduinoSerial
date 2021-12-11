#ifndef READSERIAL_H
#define READSERIAL_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFile>
#include <stdio.h>
#include <QFileInfo>

#include<stdio.h>
#include<cstdio>

#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>

#include "ringbuff.h"

//QT_CHARTS_USE_NAMESPACE
//using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class ReadSerial; }
QT_END_NAMESPACE

//struct ArduinoData{};

class ReadSerial : public QMainWindow
{
  Q_OBJECT

public:
  ReadSerial(QString fpth, QWidget *parent = nullptr);
  QString GetSerialPortName();
  QSerialPort *serial;
  void configureSerialPort(QSerialPort *serial);
  int findchar(char *xbuffer, int xlen, char c);
  void readSerialPort();
  QString fpath;// = "../Serial/data.txt";

  void openfile(QFile *file, QFlags<QIODevice::OpenMode::enum_type> mode);
  void closefile(QFile *file);

  QFile *file;
  ~ReadSerial();

  QtCharts::QLineSeries *series1, *series2;

  QValueAxis *qaxisX, *qaxisY;
  QPen makePen(Qt::GlobalColor color, int width);

  const size_t Npnt = 2000;
  RingBuff *bt, *by1, *by2;  /* ring buffers for data */

private slots:
  void on_Stop_clicked();

private:
  int xi = 0;
  Ui::ReadSerial *ui;
  void SerialError();
  int  bufsize = 1024;
  const int linemin = 4 + 3 + 2 + 1; /* 4items + 3',' + 2'{}' + 1'\n' */
  const double dy = 0.05;
  double y1last = 1, y2last = 1, ymin, ymax;
  const double y2max = 9.313225750491594e-10; /* 2/((qreal)(2^31-1)) */

};
#endif // READSERIAL_H
