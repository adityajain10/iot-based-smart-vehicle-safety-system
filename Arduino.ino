#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(9, 10); //gps
SoftwareSerial mySerial(7, 8);//gsm
SoftwareSerial iot(11, 12);//iot
float flat, flon;
unsigned long age;
int eye = 3;
int y;
int e1;
int b1;
int a = 1; //alc
int b;//alc
int c = 2; //vib
int d;//vib
int e = 3; //seat
int f;//seat
int h = 2; //prox
int j;//prox
int g = 1;
int buz = 5;
int led = 13;
int mot = 4;
void setup() {
  pinMode(a, INPUT);
  pinMode(c, INPUT);
  pinMode(y, INPUT);
  pinMode(e, INPUT); //IR SEATBELT
  pinMode(h, INPUT);
  pinMode(led, OUTPUT);
  pinMode(mot, OUTPUT);
  pinMode(buz, OUTPUT);
  Serial.begin(9600); // put your setup code here, to run once:
  iot.begin(9600);
}

void loop() {
  ss.begin(9600);
  delay(100);
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {

    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c))
      {
        newData = true;

      }
    }
  }
  if (newData)
  {
    gps.f_get_position(&flat, &flon, &age);
  }
  b = analogRead(a);
  d = analogRead(c);
  f = digitalRead(e);
  j = digitalRead(h);
  y = analogRead(eye);
  //Serial.println(f);// put your main code here, to run repeatedly:
  //   Serial.println(f);
  //   Serial.println(distance);
  if (j == HIGH) //proximity
  {
    digitalWrite(mot, LOW);
  }
  if ((b < 345) && (f == LOW) && (g == 1) && (j == LOW))
  {
    digitalWrite(led, HIGH);
    digitalWrite(mot, HIGH);
  }
  if ((b > 350) || (f == HIGH) || (g == 2))
  {
    digitalWrite(led, LOW);
    digitalWrite(mot, LOW);
  }

  if (d < 210) //vibration
  {
    digitalWrite(led, LOW);
    digitalWrite(mot, LOW);
    g = 2;
    Serial.println("ACCIDENT OCCURED");
    Serial.print("LAT ");
    Serial.println(flat, 6);
    Serial.print("LON ");
    Serial.println(flon, 6);
    Serial.println();
    iot.print("*");
    iot.print("ACCIDENT OCCURED");
    iot.print(" LAT: ");
    iot.print(flat, 6);
    iot.print(" LON: ");
    iot.print(flon, 6);
    iot.print("#");
    mySerial.begin(9600);
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"+918700253101\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("ACCIDENT OCCURED");
    mySerial.print("LAT ");
    mySerial.println(flat, 6);
    mySerial.print("LON ");
    mySerial.println(flon, 6);
    mySerial.println();
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
  }
  if ((y < 300) && (e1 == 1))
  {
    digitalWrite(buz, HIGH);
    delay(500);
    b1 = b1 + 1;
    if (b1 > 2)
    {
      digitalWrite(mot, LOW);
      Serial.println("DRIVER FEELING SLEEPY");
      iot.print("*DRIVER FEELING SLEEPY#");
      mySerial.begin(9600);
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+918700253101\"\r");
      delay(1000);
      mySerial.println("DRIVER FEELING SLEEPY");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
      e1 = 2;
      g = 2;
    }
  }
  if (y > 370)
  {
    b1 = 0;
    e1 = 1;
    digitalWrite(buz, LOW);
  }
}
