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

QT_BEGIN_NAMESPACE
namespace Ui { class ReadSerial; }
QT_END_NAMESPACE

class ReadSerial : public QMainWindow
{
  Q_OBJECT

public:
  ReadSerial(QWidget *parent = nullptr);
  QString GetSerialPortName();
  QSerialPort *serial;
  void configureSerialPort(QSerialPort *serial);
  void readSerialPort();
  QFile file;
  int xi = 0;
  QString fpath = "../Serial/data.txt";
  ~ReadSerial();

private slots:
//  void on_pushButton_clicked();

  void on_Stop_clicked();

private:
  Ui::ReadSerial *ui;
  void SerialError();
};
#endif // READSERIAL_H
