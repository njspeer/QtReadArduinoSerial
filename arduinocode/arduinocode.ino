/*
/home/nathan/Documents/Arduino/Blink_100ms/.vscode/arduino.json

You can monitor the output on your virtual serial port ttyACM0

/dev/ttyACM0

Tools/Serial Monitor in the Arduino IDE
or tail -f /dev/ttyACM0 in the Linux terminal


*/

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
int i = 0;
void loop() {
  int dt = 100;
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level) */
  delay(dt);                         // wait for dt ms */
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW */
  delay(dt);                         // wait for dt ms */

  Serial.print('<');                 /* begin data */
  Serial.print(i);                   /* data */
  Serial.print(',');                 /* data */
  Serial.print(i*10);                /* data */
  Serial.print(',');                 /* data */
  Serial.print(i*100);               /* data */
  Serial.print(',');                 /* data */
  Serial.print(i*1000);              /* data */
  Serial.print('>');                 /* end data */
  Serial.print('\n');                /* data */
  i++;                               /* increment i */ 
}
