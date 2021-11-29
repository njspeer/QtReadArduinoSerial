# QtReadArduinoSerial

Example code shows how to send data from an Arduino UNO and read it from a Qt/C++ program.


# arduinocode.ino #
Arduino code: use the Arduino IDE (or VS Code) to upload this file to the Aruduino.

Current code blinks the LED and sends a series of integers to the serial port.  The integers are formatted as such: 

"<i, 10\*i, 100\*i, 1000\*i>\n"

send 10 lines per second.

# readserial.cpp #
Main file that
1. Gets the name of the serial port
    (looks for a port name starting with 'ttyACM')
2. Opens and configures the port
3. Reads data from the port and writes it to "data.txt" (path specified in the header)

# readserial.ui #

Use the 'Stop' button to stop the collection.


