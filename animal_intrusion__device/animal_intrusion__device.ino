
#include <Ultrasonic.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;
const int trigPin1 = D3;
const int echoPin1 = D4;
const int trigPin2 = D5;
const int echoPin2 = D6;

Ultrasonic ultrasonic1(trigPin1, echoPin1);
Ultrasonic ultrasonic2(trigPin2, echoPin2);
Servo servo;
int pos = 0;
//------------------------------tw--------------
const char* ssid = "Sid";
const char* password = "299599666";
const char* host = "thingworx.scetngp.com";
const int httpsPort = 80;

const char Thing[] = "ultrasonic1";
const char Property1[] = "ultraSonic1";
const char Property2[] = "ultraSonic2";

void Put(String ThingName, String ThingProperty, int Value)
{

  Serial.println(host);
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  } else

  {
    Serial.println("Connected to ThingWorx.");
  }
  String url = "/Thingworx/Things/" + ThingName + "/Properties/" + ThingProperty;
  Serial.print("requesting URL: ");
  Serial.println(url);

  String strPUTReqVal = "{\"" + ThingProperty + "\":\"" + Value + "\"}";
  Serial.print("PUT Value: ");
  Serial.println(strPUTReqVal);

  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "appKey: 87750caa-b78d-4d14-b706-4bd8ccc9764b" + "\r\n" +
               "x-thingworx-session: false" + "\r\n" +
               "Accept: application/json" + "\r\n" +
               "Connection: close" + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: " + String(strPUTReqVal.length()) + "\r\n\r\n" +
               strPUTReqVal + "\r\n\r\n");

  while (client.connected())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
}
//--------------------tw---------------------------
void setup() {
  //---------------------------------------
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //-------------------------------------------------------
  Serial.begin(9600);
  servo.attach(D7);
  // initialize the LCD
  //lcd.begin();
  lcd.init();
  pinMode(D8, OUTPUT);
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Starting...");
}

void loop() {
  int distance1 = ultrasonic1.read();
  int distance2 = ultrasonic2.read();

  Serial.print("Sensor 1: ");
  Serial.print(distance1);
  Serial.print(" cm");

  Serial.print("  Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  for (pos = 0; pos <= 180; pos += 5)
  {
    distance1 = ultrasonic1.read();
    distance2 = ultrasonic2.read();
    if (distance1 <= 20 || distance2 <= 20)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("INTRUSION!");
      lcd.setCursor(0, 1);
      lcd.print("some thing is here!");
      digitalWrite(D8, HIGH);
      Serial.print("Sensor 1: ");
      Serial.print(distance1);
      Serial.print(" cm");

      Serial.print("  Sensor 2: ");
      Serial.print(distance2);
      Serial.println(" cm");
      //delay(50);
      //digitalWrite(D8, LOW);
      Put(Thing, Property1, distance1);
      Put(Thing, Property2, distance2);
      delay(100);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SAFE!");
      digitalWrite(D8, LOW);
      Serial.print("Sensor 1: ");
      Serial.print(distance1);
      Serial.print(" cm");

      Serial.print("  Sensor 2: ");
      Serial.print(distance2);
      Serial.println(" cm");

      delay(100);
    }
    servo.write(pos);
    delay(50);
  }



  for (pos = 180; pos >= 0; pos -= 5)
  { distance1 = ultrasonic1.read();
    distance2 = ultrasonic2.read();
    if (distance1 <= 20 || distance2 <= 20)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("INTRUSION!");
      lcd.setCursor(0, 1);
      lcd.print("some thing is here!");
      digitalWrite(D8, HIGH);
      //delay(50);
      //digitalWrite(D8, LOW);
      Put(Thing, Property1, distance1);
      Put(Thing, Property2, distance2);
      delay(100);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SAFE!");
      digitalWrite(D8, LOW);

      delay(100);
    }
    servo.write(pos);
    delay(50);

  }

}
