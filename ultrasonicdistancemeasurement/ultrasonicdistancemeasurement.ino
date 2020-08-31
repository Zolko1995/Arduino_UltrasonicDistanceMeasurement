#include <Wire.h> //Wire könyvtár beillesztése az I2C busz használatához
#include <LiquidCrystal_I2C.h> //Az I2C Folyékony kristályos LCD kijelző kezelő könyvtára
LiquidCrystal_I2C lcd(0x27, 16, 2); //Az általunk használt kijelző karakterkészlete 16 karakter és 2 sor

const int trigPin = 9; //konstans globális integer típusú változó mely az Ultrahangos szenzor trigger pin-jét tárolja
const int echoPin = 10; //konstans globális integer típusú változó mely az Ultrahangos szenzor echo pin-jét tárolja
long idotartam; //long típusú változó mely az időtartam értékét tárolja
int tavolsag; //integer típusú változó mely a távolság értékét tárolja
int blue = 11; //A kék led pinjét tartalmazó globális integer változó
int green = 12; //A zöld led pinjét tartalmazó globális integer változó
int red = 13; //A piros led pinjét tartalmazó globális integer változó
int veszely; //globális integer típusú változó

void setup()
{
  lcd.init(); //Az LCD kijelző inicializálása
  lcd.backlight(); //Az LCD kijelző háttérvilágításának bekapcsolása
  pinMode(trigPin, OUTPUT); //A szenzor trigger pinje, mint digitális kimenet
  pinMode(echoPin, INPUT); //A szenzor echo pinje, mint digitális bemenet
  pinMode(blue, OUTPUT); //A kék ledet tartalmazó pin kimenetté alakítása
  pinMode(green, OUTPUT); //A zöld ledet tartalmazó pin kimenetté alakítása
  pinMode(red, OUTPUT); //A piros ledet tartalmazó pin kimenetté alakítása
  Serial.begin(9600); //A soros porton történő kommunikáció bitrátája
}
void loop() //ciklus
{
  digitalWrite(trigPin, LOW); //A trigger pin lenullázása
  delayMicroseconds(2); //Várakozás 2 mikroszekundum ideig
  digitalWrite(trigPin, HIGH); //A trigger pin aktiválása 1-es vagy HIGH értékkel
  delayMicroseconds(10); //A trigger pin aktiválása 10 mikroszekundum ideig
  digitalWrite(trigPin, LOW); //A trigger pin deaktiválása
  idotartam = pulseIn(echoPin, HIGH); //Az echo pinre érkező jel olvasása és tárolása az időtartam változóban
  tavolsag = idotartam * 0.034 / 2; //A távolság kiszámítása, valamint értékének a tárolása a távolság változóban
  Serial.println("A távolság: "); //Megadott karakterlánc soros porton való megjelenítése
  Serial.println(tavolsag); //A változó értékenek a soros porton való megjelenítése
  lcd.setCursor(3, 0); //Kurzor pozicionálás ez esetben 3. karakter a 0. sorban
  lcd.print("A TAVOLSAG:"); //Megadott karakterlánc kiíratása
  lcd.setCursor(6, 1); //Kurzor pozicionálás ez esetben 6. karakter a 1. sorban
  lcd.print(tavolsag); //A távolság változó értékének a kiíratása
  lcd.setCursor(9, 1); //Kurzor pozicionálás ez esetben 9. karakter a 1. sorban
  lcd.print("CM"); //Megadott karakterlánc kiíratása
  if (tavolsag > 150) //Szelekció ha a mért távolság nagyobb, mint 150 cm akkor:
  {
    veszely = 3; //A változó értéke = 3
  }
  if (tavolsag < 100 & tavolsag != 50) //Szelekció ha a mért távolság kisebb, mint 100 cm és nem egyenlő 50 cm akkor:
  {
    veszely = 0; //A változó értéke = 0
  }
  if (tavolsag < 45 & tavolsag != 25) //Szelekció ha a mért távolság kisebb, mint 45 cm és nem egyenlő 25 cm akkor:
  {
    veszely = 1; //A változó értéke = 1
  }
  if (tavolsag < 20) //Szelekció ha a mért távolság kisebb, mint 20 cm akkor:
  {
    veszely = 2; //A változó értéke = 2
  }
  
  switch(veszely) //Switch case többirányú elágazás:
  {
    case 0: //Ha a veszély változó értéke = 0 akkor:
      digitalWrite(blue, HIGH); //Kék led világít
      break; //Kilépés az elágazásból
    case 1: //Ha a veszély változó értéke = 1 akkor:
      digitalWrite(green, HIGH); //Zöld led világít
      digitalWrite(blue, LOW); //Kék led nem világít
      break; //Kilépés az elágazásból
    case 2: //Ha a veszély változó értéke = 2 akkor:
      digitalWrite(red, HIGH); //Piros led világít
      digitalWrite(blue, LOW); //Kék led nem világít
      digitalWrite(green, LOW); //Zöld led nem világít
      break; //Kilépés az elágazásból
    default: //Egyébként
      digitalWrite(blue, LOW); //Kék led nem világít
      digitalWrite(green, LOW); //Zöld led nem világít
      digitalWrite(red, LOW); //Piros led nem világít
      break; //Kilépés az elágazásból
  }
  delay(500); //Várakozás 0.5 másodpercig
  lcd.clear(); //Az LCD kijelző tartalmának a törlése
}
