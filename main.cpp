#include "readserial.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ReadSerial w("../QtReadArduinoSerial/data.csv");
  w.move(200, 200);
  w.resize(1500, 1500);
  w.show();
  return a.exec();
}
