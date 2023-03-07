#include <Keypad.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "MQ7.h"
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);
SoftwareSerial mySerial(11, 10);

#define sensor A0
int buzzer = 13;
#define A_PIN 2
#define VOLTAGE 5
MQ7 mq7(A_PIN, VOLTAGE);

#include <ACI_10K_an.h>
Aci_10K an10k;
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns


int temp;
int number = 0;
//int lcd_num = 0;
String phone_number = "+98 ";
byte Character[8] = { 0b00110, 0b01110, 0b01000, 0b01000, 0b01000, 0b00111, 0b00011, 0b00000 };

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int rl = 12;
int gl = 11;

int high = 53;
int low = 52;

int high_led = 28;

int g_l = 32;
int gh = 31;

int rh = 30;
int r_l = 29;

void setup() {
  pinMode(gh, OUTPUT);
  pinMode(g_l, OUTPUT);
  pinMode(rh, OUTPUT);
  pinMode(r_l, OUTPUT);
  pinMode(high, OUTPUT);
  pinMode(high_led, OUTPUT);
  pinMode(low, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(gl, OUTPUT);
  pinMode(rl, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Calibrating.....");
  mq7.calibrate();
  lcd.print("Calibration done.");
  lcd.clear();
  lcd.print("In The Name Of");
  lcd.setCursor(0, 2);
  lcd.print("Allah");
  delay(500);
  lcd.clear();
  lcd.print("Made With Seyed Mohammad Parsa Arabi");
  delay(500);
  for (int a = 0; a < 20; a++) {
    delay(500);
    lcd.scrollDisplayLeft();
  }
  delay(500);
  update_number();
  lcd.clear();
}
void loop() {
  temp = analogRead(sensor);
  int voltage = temp / 2.8;
  digitalWrite(gh, 1);
  digitalWrite(g_l, 0);
  digitalWrite(r_l, 0);
  digitalWrite(rh, 0);
  digitalWrite(low, 0);
  digitalWrite(high, 1);
  digitalWrite(high_led, 1);
  lcd.setCursor(0, 0);
  lcd.print("Mo:");
  lcd.print(int(mq7.readPpm()));
  lcd.print(" ppm");
  update_number();
  lcd.setCursor(9, 0);
  lcd.write("|");
  lcd.setCursor(10, 0);
  lcd.write("T:");
  lcd.print(voltage);
  lcd.createChar(1, Character);
  lcd.setCursor(0, 2);
  lcd.write(byte(1));
  lcd.setCursor(2, 2);
  lcd.print(phone_number);
  if (int(mq7.readPpm()) > 10) {
    lcd.clear();
    lcd.print("home is dangerous!!");
    digitalWrite(buzzer, 1);
    digitalWrite(gh, 0);
    digitalWrite(g_l, 0);
    digitalWrite(r_l, 1);
    digitalWrite(rh, 0);
    // lcd.clear();
  }
  if (voltage > 22) {
    lcd.setCursor(0, 2);
    lcd.print(" home is hot!!  ");
  }
  if (voltage < 16) {
    lcd.setCursor(0, 2);
    lcd.print(" home is cold!! ");
  }
  char key = keypad.getKey();
  if (key) {
    lcd.display();
    click_button();
    Serial.println(key);
    if (key == 'A') {
      change();
    }
    if (key == 'B') {
      delete_eeprom();
    }
    if (key == 'C') {
      digitalWrite(buzzer, 0);
    }
    if (key == 'D'){
      lcd.clear();
    }
  }
  delay(250);
}
void send_message(char text) {
  mySerial.println("AT+CMGF=1");
  updateSerial();
  String get_phone_number = "+98";
  for (int i = 0; i < 10; i++) {
    get_phone_number += EEPROM.read(i);
  }
  mySerial.println("AT+CMGS=\"{get_phone_number}\"");
  updateSerial();
  mySerial.print(text);
  updateSerial();
  mySerial.write(26);
}

void change() {
  number = 0;
  lcd.print("set number finish");
  delay(500);
  lcd.clear();
  lcd.print("click button");
  phone_number = "+98 ";
  while (number < 10) {
    char button = keypad.getKey();
    if (phone_number) {
      Serial.println(phone_number);
      if (button == '*') {
        click_button();
        lcd.clear();
        lcd.print("click button");
        phone_number = "+98 ";
        number = 0;
      }
      if (button == '#') {
        click_button();
        break;
      }
      if (button == '0') {
        click_button();
        EEPROM.write(number, 0);
        phone_number += "0";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '1') {
        click_button();
        EEPROM.write(number, 1);
        phone_number += "1";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '2') {
        click_button();
        EEPROM.write(number, 2);
        phone_number += "2";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '3') {
        click_button();
        EEPROM.write(number, 3);
        phone_number += "3";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '4') {
        click_button();
        EEPROM.write(number, 4);
        phone_number += "4";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '5') {
        click_button();
        EEPROM.write(number, 5);
        phone_number += "5";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '6') {
        click_button();
        EEPROM.write(number, 6);
        phone_number += "6";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '7') {
        click_button();
        EEPROM.write(number, 7);
        phone_number += "7";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '8') {
        click_button();
        EEPROM.write(number, 8);
        phone_number += "8";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
      if (button == '9') {
        click_button();
        EEPROM.write(number, 9);
        phone_number += "9";
        number += 1;
        lcd.setCursor(0, 2);
        lcd.print(phone_number);
      }
    }
  }
  update_number();
  lcd.clear();
}
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //هرچه در سریال مانیتور نوشته شود به
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //هرچه از ماژول دریافت شود در سریال مانیتور نوشته میشود
  }
}

void delete_eeprom() {
  lcd.clear();
  lcd.print("delete EEPROM...");
  for (int i = 0; i < 20; i++) {
    EEPROM.write(i, 0);
  }
  lcd.print("delete EEPROM success");
  delay(1000);
  update_number();
  lcd.clear();
}
void update_number() {
  phone_number = "+98 ";
  for (int i = 0; i < 10; i++) {
    phone_number += EEPROM.read(i);
  }
}
void click_button() {
  digitalWrite(buzzer, 1);
  delay(250);
  digitalWrite(buzzer, 0);
}
