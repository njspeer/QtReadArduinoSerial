QT      += core gui serialport widgets charts
CONFIG  += c++17
SOURCES += main.cpp readserial.cpp \
  ringbuff.cpp
HEADERS += readserial.h \
  ringbuff.h \
  ringbuff.h
FORMS   += readserial.ui


