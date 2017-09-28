#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Narcoleptic.h>

//Define a ligação ao pino de dados do sensor
#define DHTPIN A3

//Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT11

// I2C address - Find it from I2C Scanner.
#define I2C_ADDR    0x27
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

// Speaker pin
#define SPK_PIN 12
#define SPK_TONE 294
#define SPK_VOL 30

// Humidity sensor pin
#define HUMIDITY_SENSOR_1 A0
#define HUMIDITY_SENSOR_2 A1

// Minimum humidity acceptable
#define MIN_HUMIDITY 70

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

//Array simbolo grau
byte grau[8] = { B00001100,
                 B00010010,
                 B00010010,
                 B00001100,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000,
               };

void setup()
{
  Narcoleptic.disableSerial();
  Narcoleptic.disableADC();
  Narcoleptic.disableSPI();

  lcd.begin (16, 2);
  lcd.clear();

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home

  //Cria o caractere customizado com o simbolo do grau
  lcd.createChar(0, grau);

  // iniciando buzzer
  pinMode(SPK_PIN, OUTPUT);
}

void loop()
{
  lcd.setBacklight(HIGH);

  temp();

  Narcoleptic.delay(8000);

  lcd.clear();

  humidity();

  Narcoleptic.delay(5000);

  sleep();
}

void beep() {
  for (int x = 0; x < 5; x++) {
    tone(SPK_PIN, SPK_TONE, SPK_VOL);
    delay(500);
  }
}

void sleep() {
  lcd.clear();
  lcd.setBacklight(LOW);

  for (int pausas = 0; pausas <= 5; pausas++) {
    Narcoleptic.delay(8000);
  }
}

void temp() {
  //Le o valor da umidade
  float h = dht.readHumidity();

  //Le o valor da temperatura
  float t = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7, 0);
  lcd.print(t, 1);
  lcd.setCursor(SPK_PIN, 0);

  //Mostra o simbolo do grau formado pelo array
  lcd.write((byte)0);

  lcd.setCursor(0, 1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7, 1);
  lcd.print(h, 1);
  lcd.setCursor(SPK_PIN, 1);
  lcd.print("%");
}

void humidity() {
  //Le os sensores de umidade
  int sensor1 = map(analogRead(HUMIDITY_SENSOR_1), 1023, 0, 0, 100);
  int sensor2 = map(analogRead(HUMIDITY_SENSOR_2), 1023, 0, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Sensor 1 : ");
  lcd.print(" ");
  lcd.setCursor(11, 0);
  lcd.print(sensor1);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Sensor 2 : ");
  lcd.print(" ");
  lcd.setCursor(11, 1);
  lcd.print(sensor2);
  lcd.print("%");

  if (sensor1 >= MIN_HUMIDITY || sensor2 >= MIN_HUMIDITY) {
    beep();
  }
}

