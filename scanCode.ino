#include <SoftwareSerial.h>

// Declaring Pins
#define ledPin 3
#define pSwitch 2
#define pinRX 10
#define pinTX 11

// Declaring Others
int buttonStatePrev = 0;
int buttonStateCurrent = 0;
int toggle = 0;

// Sensor
unsigned char data_buffer[4] = {0};           // Array to store incoming serial data
int distance = 0;                             // Integer to store distance
unsigned char CS;                             // Variable to hold checksum
SoftwareSerial mySerial(pinRX, pinTX);        // Object to represent software serial port

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pSwitch, INPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  int count = 0;
  int avgDistance = 0;
  while (count < 20) {
    distance = scan();
    Serial.print("distance: ");
    Serial.print(distance);
    Serial.println(" mm");
    count = count + 1;
    avgDistance = avgDistance + distance;
  }

  // Lora string
  delay(2000);
  distance = avgDistance/20;
  delay(2000);
  String potmes = String(distance);
  String cmd = "AT+SEND=1,"+String(potmes.length())+","+String(distance)+"\r";
  Serial.println(cmd);
  delay(1000);

}

int scan() {
    mySerial.listen();
    delay(4);
    if (mySerial.available() > 0) {
    delay(4);
    // Check for packet header character 0xff
    if (mySerial.read() == 0xff) {
      // Insert header into array
      data_buffer[0] = 0xff;
      // Read remaining 3 characters of data and insert into array
      for (int i = 1; i < 4; i++) {
        data_buffer[i] = mySerial.read();
      }
      //Compute checksum
      CS = data_buffer[0] + data_buffer[1] + data_buffer[2];
      // If checksum is valid compose distance from data
      if (data_buffer[3] == CS) {
        distance = (data_buffer[1] << 8) + data_buffer[2];
      }
    }
  }
  return distance;
}
