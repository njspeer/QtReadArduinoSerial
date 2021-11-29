#include "readserial.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ReadSerial w;
  w.move(200, 200);
  w.show();
  return a.exec();
}
