#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Define a ligação ao pino de dados do sensor
#define DHTPIN A3

//Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define HUMIDITY_SENSOR_1 A0
#define HUMIDITY_SENSOR_2 A1

#define MIN_HUMIDITY 70

int n = 1;

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
  Serial.begin(9600); //Inicializa a serial

  lcd.begin (16, 2); //  <<----- My LCD was 16x2
  lcd.clear(); //Limpa o LCD


  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home

  //Cria o caractere customizado com o simbolo do grau
  lcd.createChar(0, grau);
}

void loop()
{
  float h = dht.readHumidity(); //Le o valor da umidade
  float t = dht.readTemperature(); //Le o valor da temperatura
  
  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7, 0);
  lcd.print(t, 1);
  lcd.setCursor(12, 0);

  //Mostra o simbolo do grau formado pelo array
  lcd.write((byte)0);

  //Mostra o simbolo do grau quadrado
  //lcd.print((char)223);

  lcd.setCursor(0, 1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7, 1);
  lcd.print(h, 1);
  lcd.setCursor(12, 1);
  lcd.print("%");

  //Intervalo recomendado para leitura do sensor
  delay(2000);

  lcd.clear();

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

  delay(2000);

  lcd.clear();
}
