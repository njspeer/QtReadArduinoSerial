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

  const int Npnt = 2000;
  Vbuff<double> *b;
//  Vbuff<qreal> ringbuff;

private slots:
  void on_Stop_clicked();

private:
  int xi = 0;
  Ui::ReadSerial *ui;
  void SerialError();
  qint64 bufsize = 1024;
  const int linemin = 4 + 3 + 2 + 1; /* 4items + 3',' + 2'{}' + 1'\n' */
  const qreal dy = 0.05;
  qreal tlast = 0, y1last = 1, y2last = 1, ymin = 1, ymax = 1;
  const qreal y2max = 9.313225750491594e-10; /* 2/((qreal)(2^31-1)) */

};
#endif // READSERIAL_H
