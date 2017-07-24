#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
#include <Keypad.h>
#include <Servo.h>

#define kLed 10
#define yLed 11
#define buzzer 12
#define servo 13

int sifreSayac = 0;
int lock = 1;

char basilanTus;
char tus_Takimi[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte satir_pinleri[4] = {2, 3, 4, 5};
byte sutun_pinleri[4] = {6, 7, 8, 9};
byte sifre[4] = {0, 0, 0, 0};
byte girilenSifre[4] = {0, 0, 0, 0};
byte eskiSifre[4] = {0, 0, 0, 0};
byte ilkSifre[4] = {0, 0, 0, 0};
byte ikinciSifre[4] = {0, 0, 0, 0};

Keypad keypad = Keypad(makeKeymap(tus_Takimi), satir_pinleri, sutun_pinleri, 4, 4);
LiquidCrystal_I2C_AvrI2C lcd(0x3f, 16, 2);
Servo sg90;

void setup() {
  pinMode(kLed, OUTPUT);
  pinMode(yLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(kLed, HIGH);
  digitalWrite(yLed, HIGH);
  sg90.attach(servo);
  sg90.write(0);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  basilanTus = keypad.getKey();
  if (lock == 1) {
    ilk_Ekran();
  }
  else if (lock == 0) {
    tus_Kontrol();
  }
  else if (lock == 2) {
    sifre_Degistir();
  }
}

void sifre_Degistir() {
  lcd.setCursor(3, 0);
  lcd.print("ESKI SIFRE");
  if (basilanTus) {
    beep();
    eskiSifre[sifreSayac] = basilanTus;
    Serial.println(sifreSayac);
    Serial.print(eskiSifre[0]);
    Serial.print(eskiSifre[1]);
    Serial.print(eskiSifre[2]);
    Serial.println(eskiSifre[3]);
    lcd.setCursor(sifreSayac + 6, 1);
    lcd.print('*');
    if (sifreSayac == 3) {
      if (strncmp(sifre, eskiSifre, 4) == 0) {
        delay(200);
        sifreSayac = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SIFRELER UYUSTU");
        digitalWrite(yLed, LOW);
        dogruBeep();
        delay(1000);
        digitalWrite(yLed, HIGH);
        lcd.clear();
        lock = 1;
      }
      else {
        delay(200);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("SIFRELER");
        lcd.setCursor(4, 1);
        lcd.print("UYUSMADI");
        digitalWrite(kLed, LOW);
        yanlisBeep();
        delay(1000);
        digitalWrite(kLed, HIGH);
        sifreSayac = 0;
        lock = 0;
      }
      sifreSayac = 0;
    }
    else {
      sifreSayac++;
    }
  }
}

void ilk_Ekran() {
  Serial.println(sifreSayac);
  lcd.setCursor(1, 0);
  lcd.print("SIFRE BELIRLE");
  if (basilanTus) {
    beep();
    sifre[sifreSayac] = basilanTus;
    lcd.setCursor(sifreSayac + 6, 1);
    lcd.print('*');
    if (sifreSayac == 3) {
      delay(200);
      lcd.setCursor(0, 0);
      lcd.print("SIFRE BELIRLENDI");
      digitalWrite(yLed, LOW);
      dogruBeep();
      lock = 0;
      sifreSayac = 0;
      delay(1000);
      digitalWrite(yLed, HIGH);
      lcd.clear();
    }
    else {
      sifreSayac++;
    }
    Serial.print(sifre[0]);
    Serial.print(sifre[1]);
    Serial.print(sifre[2]);
    Serial.println(sifre[3]);
  }
}

void tus_Kontrol() {
  lcd.setCursor(0, 0);
  lcd.print("GUVENLIK SISTEMI");
  lcd.setCursor(0, 1);
  lcd.print("SIFRE: ");
  if (basilanTus == 'A') {
    lcd.clear();
    sifreSayac = -1;
    lock = 2;
    delay(150);
  }
  if (basilanTus) {
    beep();
    char tus = basilanTus;
    girilenSifre[sifreSayac] = basilanTus;
    lcd.setCursor(sifreSayac + 7 , 1);
    lcd.print('*');
    sifreSayac ++;
    Serial.print(sifre[0]);
    Serial.print(sifre[1]);
    Serial.print(sifre[2]);
    Serial.println(sifre[3]);
    Serial.print(girilenSifre[0]);
    Serial.print(girilenSifre[1]);
    Serial.print(girilenSifre[2]);
    Serial.println(girilenSifre[3]);
    Serial.println("----------------");
    if (strncmp(sifre, girilenSifre, 4) == 0) {
      delay(250);
      digitalWrite(yLed , LOW);
      Serial.println("Sifre Kabul Edildi");
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("SIFRE KABUL");
      lcd.setCursor(5, 1);
      lcd.print("EDILDI");
      sg90.write(180);
      sifreSayac = 0;
      girilenSifre[0] = 0;
      girilenSifre[1] = 0;
      girilenSifre[2] = 0;
      girilenSifre[3] = 0;
      dogruBeep();
      delay(2000);
      sg90.write(0);
      digitalWrite(yLed, HIGH);
      lcd.clear();
    }
    else if (sifreSayac == 4) {
      delay(250);
      digitalWrite(kLed, LOW);
      Serial.println("Sifre Yanlis Girildi");
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("SIFRE YANLIS");
      lcd.setCursor(5, 1);
      lcd.print("GIRILDI");
      sifreSayac = 0;
      girilenSifre[0] = 0;
      girilenSifre[1] = 0;
      girilenSifre[2] = 0;
      girilenSifre[3] = 0;
      yanlisBeep();
      delay(1300);
      digitalWrite(kLed, HIGH);
      lcd.clear();
    }
  }
}
void dogruBeep() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}

void yanlisBeep() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(150);
  digitalWrite(buzzer, LOW);
}

void beep() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}

