// e3702iot@ptct.net
// Info@2025

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#include <LCD_I2C.h>

LCD_I2C lcd(0x27);

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL3buUe8_-3"
#define BLYNK_TEMPLATE_NAME "construction monitoring"
#define BLYNK_AUTH_TOKEN "l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv"

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "IOT";
char pass[] = "123456789";

#include "DHT.h"      // including the library of DHT11 temperature and humidity sensor
#define DHTPIN 4      // Selecting the pin at which we have connected DHT11
#define DHTTYPE DHT11 // Selecting the type of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

int humidity, temp1;

#include "HX711.h"
const int dout = 18;
const int sck = 5;

HX711 scale;
float kg, p;

String receivedSrting;
int buttonread, act = 0, sec = 0;

int accx, accy, accz, acc;

#define mois_sensor 34
#define vib_sensor 2
int mois_value, vib_value, mois_final;
void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(vib_sensor, INPUT);
    pinMode(mois_sensor, INPUT);

    lcd.begin();
    lcd.backlight();

    scale.begin(dout, sck);
    scale.read();
    scale.read_average(20);
    scale.get_value(5);
    (scale.get_units(5), 1);
    scale.set_scale(2280.f);
    scale.tare();
    scale.read();
    scale.read_average(20);
    scale.get_value(5);
    (scale.get_units(5), 1);

    Serial.println("Adafruit MLX90614 test");
    lcd.setCursor(0, 0);
    lcd.print("CONSTRUCTION "); // You can make spaces using well... spaces
    lcd.setCursor(0, 1);        // Or setting the cursor in the desired position.
    lcd.print("MONITORING ");
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
    lcd.clear();
}

void loop()
{
    Blynk.run();

    vib_value = digitalRead(vib_sensor);
    mois_value = analogRead(mois_sensor);

    Serial.print("mois_value : ");
    Serial.println(mois_value);

    mois_value = map(mois_value, 0, 4095, 100, 0);
    if (mois_value <= 9)
    {
        mois_value = 0;
    }
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    /*Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");*/

    acc = a.acceleration.z;
    acc = map(acc, -12, 12, 0, 10);

    accx = a.acceleration.x;
    accx = map(accx, -7, 7, 0, 10);

    accy = a.acceleration.y;
    accy = map(accy, -10, 10, 0, 10);

    (scale.get_units(), 1);
    p = scale.get_units(10);
    Serial.print("p:");
    Serial.println(p);
    (p, 1);
    if (p <= 0)
    {
        p = p * (-1);
        Serial.println("positive conversion");
    }
    kg = (p * 4.67);

    if (kg < 0.1)
    {
        kg = 0;
    }
    scale.power_down(); // put the ADC in sleep mode
    delay(1000);
    scale.power_up();

    Serial.print("Kg : ");
    Serial.println(kg);
    if (kg <= 3)
    {
        kg = 0;
    }

    humidity = dht.readHumidity(); // Declaring h a variable and storing the humidity in it.
    temp1 = dht.readTemperature(); // Declaring t a
    Serial.print("temp  :");
    Serial.println(temp1);
    Serial.print(" humidity  :");
    Serial.println(humidity);

    lcd.setCursor(0, 0);
    lcd.print("A:");
    if (acc <= 9)
    {
        lcd.print("0");
        lcd.print(acc);
    }
    else if (acc <= 99)
    {
        lcd.print("");
        lcd.print(acc);
    }

    lcd.setCursor(5, 0);
    lcd.print("T:");
    if (temp1 <= 9)
    {
        lcd.print("0");
        lcd.print(temp1);
    }
    else if (temp1 <= 99)
    {
        lcd.print("");
        lcd.print(temp1);
    }

    lcd.setCursor(10, 0);
    lcd.print("H:");
    if (humidity <= 9)
    {
        lcd.print("0");
        lcd.print(humidity);
    }
    else if (humidity <= 99)
    {
        lcd.print("");
        lcd.print(humidity);
    }

    lcd.setCursor(8, 1);
    lcd.print("M:");
    if (mois_value <= 9)
    {
        lcd.print("00");
        lcd.print(mois_value);
    }
    else if (mois_value <= 99)
    {
        lcd.print("0");
        lcd.print(mois_value);
    }
    else if (mois_value <= 999)
    {
        lcd.print(mois_value);
    }

    lcd.setCursor(0, 1);
    lcd.print("W:");
    if (kg <= 9)
    {
        lcd.print("00");
        lcd.print(kg);
    }
    else if (kg <= 99)
    {
        lcd.print("0");
        lcd.print(kg);
    }
    else if (kg <= 999)
    {
        lcd.print(kg);
    }

    //
    lcd.setCursor(13, 1);
    lcd.print("V:");

    if (acc >= 1 && acc <= 5)
    {
        Serial.println("FALL DETECTED ");
        Blynk.virtualWrite(V6, "FALL DETECTED");
        Blynk.logEvent("msg", "FALL DETECTED");
    }

    else if (vib_value == 1)
    {
        lcd.setCursor(15, 1);
        lcd.print("D");
        Serial.println("VIBRATION  detecton  ");
        Blynk.logEvent("msg", "VIBRATION  DETECTED");
        Blynk.virtualWrite(V6, "VIBRATION  DETECTED ");
    }
    else if (mois_value >= 10 && mois_value <= 20)
    {
        Serial.println("MOISTURE LEVEL LOW ");
        Blynk.virtualWrite(V6, "MOISTURE LEVEL LOW");
        Blynk.logEvent("msg", "MOISTURE LEVEL LOW");
    }
    else if (mois_value >= 80)
    {
        Serial.println("MOISTURE LEVEL HIGH ");
        Blynk.virtualWrite(V6, "MOISTURE LEVEL HIGH");
        Blynk.logEvent("msg", "MOISTURE LEVEL HIGH");
    }
    else
    {
        lcd.setCursor(15, 1);
        lcd.print("N");
        Blynk.virtualWrite(V6, "                                   ");
    }
    if (mois_value >= 10)
    {
        mois_final = mois_value;
    }

    if (buttonread == 1)
    {

        Blynk.virtualWrite(V0, temp1);
        Blynk.virtualWrite(V1, humidity);
        Blynk.virtualWrite(V2, mois_final);
        Blynk.virtualWrite(V3, kg);
        Blynk.virtualWrite(V4, vib_value);
        Blynk.virtualWrite(V9, acc);
        Blynk.virtualWrite(V10, accx);
        Blynk.virtualWrite(V11, accy);
    }
    delay(500);
}
BLYNK_WRITE(V5)
{
    int button = param.asInt();
    Serial.print("robot Button value :");
    Serial.println(button);
    buttonread = button;
}

BLYNK_WRITE(V7)
{
    String received = param.asStr();
    Serial.print("robot Button value :");
    Serial.println(received);
    receivedSrting = received;
    Blynk.virtualWrite(V8, receivedSrting);
    delay(3000);
    Blynk.virtualWrite(V8, "                          ");
}
/*e3702
GET LINK

https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V0=temperature
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V1=humidity
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V2=mois_value
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V3=kg
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V4=vib sensor_value
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V9= accelerometer Z VALUE
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V10= accelerometer X VALUE
https://blynk.cloud/external/api/get?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V11= accelerometer y VALUE
UPDATE LINK

https://blynk.cloud/external/api/update?token=l_SxtC7B4mS-2DViwSM6rXGxXtcnPSCv&V7=*/
