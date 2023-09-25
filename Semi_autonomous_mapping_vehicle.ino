#include <SPI.h>
#include <SD.h>
#include <Servo.h>
/*
SD card attached to SPI bus:
MOSI - pin 11 
MISO - pin 12 
CLK - pin 13 
CS - depends on your SD card shield or module. Pin 4 used here for consistency with Arduino examples
*/
#define trigPin 8
#define echoPin 7
#define ser 9
#define en1 A0
#define en2 A1
#define en3 A2
#define en4 A3
#define leftTrack 5
#define rightTrack 6
#define buttonA 2
#define buttonB 3
Servo myser;
File myFile;
long range;
const int chipSelect = 4;
int pos = 90;                     //servo position
unsigned long x, y;               //values in calculate function
unsigned long xp = 250;           // row position
unsigned long yp = 500;           // column position
unsigned long byteValue = 0;      // byte to replacy in file
unsigned long height = 500;       //row amount
unsigned long length = 1000;      //column amount
unsigned long textLength = 1002;  // column with line breaks
byte workMode = 0;                // 0- stop; 1- run
byte direction = 1;               //1-up;2-left;3-back;4-right;
void turnleft() {
  analogWrite(leftTrack, 200);
  analogWrite(rightTrack, 200);
  digitalWrite(en2, HIGH);
  digitalWrite(en1, LOW);
  digitalWrite(en3, HIGH);
  digitalWrite(en4, LOW);
  delay(1220);
  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  digitalWrite(en3, LOW);
  digitalWrite(en4, LOW);
  if (direction == 1) {  //from up to left
    xp = xp + 4;
    yp = yp - 18;
  }
  if (direction == 2) {  // from left to back
    xp = xp + 9;
    yp = yp + 8;
  }
  if (direction == 3) {  //from back to right
    xp = xp - 4;
    yp = yp + 18;
  }
  if (direction == 4) {  //from right to up
    xp = xp - 9;
    yp = yp - 8;
  }
  direction++;
  if (direction == 5) {
    direction = 1;
  }
}
void turnright() {
  analogWrite(leftTrack, 200);
  analogWrite(rightTrack, 200);
  digitalWrite(en1, HIGH);
  digitalWrite(en2, LOW);
  digitalWrite(en4, HIGH);
  digitalWrite(en3, LOW);
  delay(1220);
  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  digitalWrite(en3, LOW);
  digitalWrite(en4, LOW);
  if (direction == 1) {  //from up to right
    xp = xp + 4;
    yp = yp + 18;
  }
  if (direction == 4) {  //from right to back
    xp = xp + 9;
    yp = yp - 8;
  }
  if (direction == 3) {  //from back to left
    xp = xp - 4;
    yp = yp - 18;
  }
  if (direction == 2) {  //from left to up
    xp = xp - 9;
    yp = yp + 8;
  }
  direction--;
  if (direction == 0) {
    direction = 4;
  }
}
int measure() {
  long tim, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  tim = pulseIn(echoPin, HIGH);
  distance = tim / 58;
  return distance;
}
void drive() {
  int disfront, disleft, disright;
  myser.write(90);
  delay(200);
  disfront = measure();
  if (disfront > 20) {
    analogWrite(leftTrack, 200);
    analogWrite(rightTrack, 200);
    digitalWrite(en1, HIGH);
    digitalWrite(en2, LOW);
    digitalWrite(en3, HIGH);
    digitalWrite(en4, LOW);
    delay(350);
    digitalWrite(en1, LOW);
    digitalWrite(en2, LOW);
    digitalWrite(en3, LOW);
    digitalWrite(en4, LOW);
    switch (direction) {
      case 1: xp = xp - 10; break;
      case 2: yp = yp - 20; break;
      case 3: xp = xp + 10; break;
      case 4: yp = yp + 20; break;
      default: break;
    }
  } else {
    delay(400);
    myser.write(178);
    delay(400);
    disleft = measure();
    myser.write(2);
    delay(400);
    disright = measure();
    myser.write(90);
    delay(100);
    if (disright > disleft) {
      turnright();
    } else {
      turnleft();
    }
  }
}
void calculate() {
  if (range <= 40) {
    if (pos >= 90) {
      x = range * cos((pos - 90) * PI / 180);
      y = range * sin((pos - 90) * PI / 180);
      if (direction == 1 && xp > x + 1 && yp > 2 * y) {
        byteValue = textLength * (xp - x - 1) + yp - 2 * y;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 2 && height > xp + y - 1 && yp > 2 * x) {
        byteValue = textLength * (xp + y - 1) + yp - 2 * x;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 3 && height > xp + x - 1 && length > yp + 2 * y) {
        byteValue = textLength * (xp + x - 1) + yp + 2 * y;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 4 && xp > y + 1 && length > yp + 2 * x) {
        byteValue = textLength * (xp - y - 1) + yp + 2 * x;
        myFile.seek(byteValue);
        myFile.print("11");
      }
    }
    if (pos < 90) {
      x = range * cos((90 - pos) * PI / 180);
      y = range * sin((90 - pos) * PI / 180);
      if (direction == 1 && xp > x + 1 && length > yp + 2 * y) {
        byteValue = textLength * (xp - x - 1) + yp + 2 * y;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 2 && xp > y + 1 && yp > 2 * x) {
        byteValue = textLength * (xp - y - 1) + yp - 2 * x;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 3 && height > xp + x - 1 && yp > 2 * y) {
        byteValue = textLength * (xp + x - 1) + yp - 2 * y;
        myFile.seek(byteValue);
        myFile.print("11");
      }
      if (direction == 4 && height > xp - y - 1 && length > yp + 2 * x) {
        byteValue = textLength * (xp + y - 1) + yp + 2 * x;
        myFile.seek(byteValue);
        myFile.print("11");
      }
    }
  }
}
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);
  pinMode(en4, OUTPUT);
  pinMode(leftTrack, OUTPUT);
  pinMode(rightTrack, OUTPUT);
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  digitalWrite(en3, LOW);
  digitalWrite(en4, LOW);
  analogWrite(leftTrack, 200);
  analogWrite(rightTrack, 200);
  myser.attach(ser);
  myser.write(pos);

  if (!SD.begin(4)) {
    while (1)
      ;
  }
  myFile = SD.open("mapa.txt", O_READ | O_WRITE | O_CREAT);
  // if the file opened okay, write to it:
  if (myFile) {

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < length; j++) {
        myFile.print(" ");
      }
      myFile.println();
    }
    myFile.close();
  }
  //servo do movement to inform that empty map was printed
  myser.write(170);
  delay(200);
  myser.write(90);
  delay(200);
}

void loop() {
  if (workMode == 0 && digitalRead(buttonA) == HIGH) {
    myFile = SD.open("mapa.txt", O_READ | O_WRITE | O_CREAT);  //open file after press button A
    delay(1000);
    workMode = 1;
  }
  if (myFile) {
    byteValue = textLength * (xp - 1) + yp;
    myFile.seek(byteValue);
    myFile.print("x");  //print vehicle positions
    pos = 178;
    while (pos >= 2) {
      if (pos < 179 && pos > 1)
        myser.write(pos);
      delay(200);
      range = measure();
      calculate();
      pos = pos - 5;
      delay(20);
      if (workMode == 1 && digitalRead(buttonB) == HIGH) {  //close file and save changes after press button B
        myFile.close();
        myser.write(90);
        pos = 1;
        delay(1000);
        workMode = 0;
      }
    }
    if (workMode == 1) drive();
  }
}