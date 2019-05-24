#include <Time.h> 
#include <DS1307RTC.h>

#include <RGBmatrixPanel.h>

#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define RELAY_OFF   40
#define RELAY_ON    41
#define BLUE_1      15
#define RED_1       16
#define GREEN_1     17
#define BLUE_2      A13
#define RED_2       A14
#define GREEN_2     A15
#define PinA        13
#define PinB        14
#define PinC        A11
#define PinD        A12

String        zeitanzeige, datumanzeige;
int reading = 0, uhr_minuten, uhr_stunden, uhr_sekunden, uhr_tag, uhr_monat, uhr_jahr;
unsigned long currentTime;
unsigned long lastTime, letzteUhrZeit;

boolean encA, encB, encC, encD;
boolean lastAB = false, lastCD = false;
int counter;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  pinMode       (32,  INPUT);           // verkackt Fix Matrix
  pinMode       (33,  INPUT);   
  pinMode       (34,  INPUT); 
  pinMode       (35,  INPUT); 
  pinMode       (36,  INPUT); 
  pinMode       (37,  INPUT); 
  
  pinMode       (RELAY_ON,  OUTPUT);    // Relay für Display vorbereiten
  digitalWrite  (RELAY_ON,  LOW);
  pinMode       (RELAY_OFF, OUTPUT);  
  digitalWrite  (RELAY_OFF, LOW);
  DISPLAY_OFF();
  
  pinMode       (BLUE_1,    OUTPUT);    // LED's vorbereiten
  digitalWrite  (BLUE_1,    HIGH);
  pinMode       (RED_1,     OUTPUT);   
  digitalWrite  (RED_1,     HIGH);
  pinMode       (GREEN_1,   OUTPUT);    
  digitalWrite  (GREEN_1,   HIGH);
  pinMode       (BLUE_2,    OUTPUT);    // LED's vorbereiten
  digitalWrite  (BLUE_2,    HIGH);
  pinMode       (RED_2,     OUTPUT);   
  digitalWrite  (RED_2,     HIGH);
  pinMode       (GREEN_2,   OUTPUT);    
  digitalWrite  (GREEN_2,   HIGH);

  pinMode       (PinA, INPUT_PULLUP);     // Rotary pins as inputs with internal pullups
  pinMode       (PinB, INPUT_PULLUP);
  pinMode       (PinC, INPUT_PULLUP);
  pinMode       (PinD, INPUT_PULLUP);

  currentTime = millis();
  lastTime = currentTime; 
  letzteUhrZeit = currentTime; 
  
  Serial.begin(9600); 
  
  LED(1,1,1000);
  LED(4,4,1000);
  LED(2,2,1000);
  LED(7,7,1000);



    
  DISPLAY_ON();
  matrix.begin();

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  char *str = "AdafruitIndustries";
  for (w=0; w<8; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.setCursor(2, 8);    // next line
  for (w=8; w<18; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.println();
  //matrix.setTextColor(matrix.Color333(4,4,4));
  //matrix.println("Industries");
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("LED MATRIX!");

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0));
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print('4');
  matrix.setCursor(34, 24);
  matrix.setTextColor(matrix.Color333(0,7,7));
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7));
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7));
  matrix.print('B');
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.print('*');
  delay(1000);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  counter = 0;
  PrintTime();
}

void loop() {


UHR();



ROTARY();

}


void UHR() {
  currentTime = millis();
  if(currentTime >= (letzteUhrZeit + 1000)){
    if(hour() != uhr_stunden){
    int dezi_st = hour() / 10;
    int basis_st = hour() - (dezi_st * 10);  
    printUhrDigit(dezi_st,3,3,7,7,7,2,10,14);  
    printUhrDigit(basis_st,14,3,7,7,7,2,10,14);     
    uhr_stunden = hour(); 
    matrix.setTextSize(2);
    matrix.setCursor(21, 3);   
    matrix.print(":");}
    
    if(minute() != uhr_minuten){
    int dezi_min = minute() / 10;
    int basis_min = minute() - (dezi_min * 10);
    printUhrDigit(dezi_min,28,3,7,7,7,2,10,14);  
    printUhrDigit(basis_min,39,3,7,7,7,2,10,14);     
    uhr_minuten = minute();}
    
    if(second() != uhr_sekunden){
    int dezi_sek = second() / 10;
    int basis_sek = second() - (dezi_sek * 10);
    printUhrDigit(dezi_sek,50,10,7,7,7,1,5,8);  
    printUhrDigit(basis_sek ,56,10,7,7,7,1,5,8);     
    uhr_sekunden = second();}

    if(day() != uhr_tag){
    int dezi_tag = day() / 10;
    int basis_tag = day() - (dezi_tag * 10);
    printUhrDigit(dezi_tag,6,22,7,7,7,1,5,8);  
    printUhrDigit(basis_tag,12,22,7,7,7,1,5,8);     
    uhr_tag = day();
    matrix.setTextSize(1);
    matrix.setCursor(16, 22);   
    matrix.print(".");}

    if(month() != uhr_monat){
    int dezi_monat = month() / 10;
    int basis_monat = month() - (dezi_monat * 10);
    printUhrDigit(dezi_monat,21,22,7,7,7,1,5,8);  
    printUhrDigit(basis_monat ,27,22,7,7,7,1,5,8);     
    uhr_monat = month();
    matrix.setTextSize(1);   
    matrix.setCursor(31, 22);   
    matrix.print(".");}

    if(year() != uhr_jahr){
    int kilo_jahr = year() / 1000;
    int hekto_jahr = (year() - (kilo_jahr * 1000)) / 100;
    int dezi_jahr = (year() - (kilo_jahr * 1000) - (hekto_jahr * 100)) /10;
    int basis_jahr = year() - (kilo_jahr * 1000) - (hekto_jahr * 100) - (dezi_jahr * 10);
    printUhrDigit(kilo_jahr,36,22,7,7,7,1,5,8);  
    printUhrDigit(hekto_jahr,42,22,7,7,7,1,5,8);     
    printUhrDigit(dezi_jahr,48,22,7,7,7,1,5,8);  
    printUhrDigit(basis_jahr,54,22,7,7,7,1,5,8);     
    uhr_jahr = year();}



//    Serial.print(month());

 //   Serial.print(year());

   



    
  }





  
}


void printUhrDigit(int Digit, int x, int y, int R, int G, int B, int s, int z1, int z2){
     matrix.fillRect(x, y, z1, z2, matrix.Color333(0, 0, 0));
     matrix.setCursor(x, y);    // start at top left, with 8 pixel of spacing
     matrix.setTextSize(s);     // size 1 == 8 pixels high
     matrix.setTextColor(matrix.Color333(R,G,B));
     matrix.print(Digit);
}
void ROTARY() {
  currentTime = millis();
  if(currentTime >= (lastTime + 5))
  {
    encA = digitalRead(PinA);
    encB = digitalRead(PinB);
    if ((!encA) && (lastAB)){if (encB){reading = reading + 1;}else{reading = reading - 1;}
      Serial.println(reading);
    }
    lastAB = encA;
    encC = digitalRead(PinC);
    encD = digitalRead(PinD);
    if ((!encC) && (lastCD)){if (encD){reading = reading + 10;}else{reading = reading - 10;}
      Serial.println(reading);
    }
    lastCD = encC;
    lastTime = currentTime;
  }
}

void PrintTime() {

  setSyncProvider(RTC.get); //Dies ist die Funktion um die Zeit- und Datumsangabe von der RTC zu bekommen

  Serial.print(hour()); //Serial.print ist der Befehl etwas im seriellen Monitor anzuzeigen (Stunde, Minute, Sekunde, Leerzeichen, Tag, Leerzeichen, usw.)
  printDigits(minute()); //bei den Minuten und Sekunden wird der Befehl
  printDigits(second()); //printDigits angegeben welcher am Ende des Codes noch festgelegt wird.
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
  delay(2000); //warte eine Sekunde
}

// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}

int LED(int LED1, int LED2, double zeit){
  if (bitRead(LED1, 0)== 1)   digitalWrite(BLUE_1, LOW);  
  if (bitRead(LED1, 1)== 1)   digitalWrite(RED_1, LOW);    
  if (bitRead(LED1, 2)== 1)   digitalWrite(GREEN_1, LOW);   
  if (bitRead(LED2, 0)== 1)   digitalWrite(BLUE_2, LOW);    
  if (bitRead(LED2, 1)== 1)   digitalWrite(RED_2, LOW);   
  if (bitRead(LED2, 2)== 1)   digitalWrite(GREEN_2, LOW);    
  delay(zeit); 
  LED_OFF();
}

void LED_OFF() {
  digitalWrite  (BLUE_1,    HIGH);  
  digitalWrite  (RED_1,     HIGH);   
  digitalWrite  (GREEN_1,   HIGH);
  digitalWrite  (BLUE_2,    HIGH);
  digitalWrite  (RED_2,     HIGH);
  digitalWrite  (GREEN_2,   HIGH);
}

void DISPLAY_ON() {
  digitalWrite(RELAY_ON, HIGH); 
  delay(50); 
  digitalWrite(RELAY_ON, LOW); 
}

void DISPLAY_OFF() {
  digitalWrite(RELAY_OFF, HIGH); 
  delay(50); 
  digitalWrite(RELAY_OFF, LOW); 
}

void printDigits(int digits) //In diesem Abschnitt wird festgelgt, dass bei Zahlen <10 im seriellen Monitor automatisch eine 0 vor den Ziffern angezeigt wird. Das gilt nur für den seriellen Monitor und nicht für LCD Display.
{ 
Serial.print(":"); 
if(digits < 10) 
Serial.print('0'); 
Serial.print(digits); 
}
