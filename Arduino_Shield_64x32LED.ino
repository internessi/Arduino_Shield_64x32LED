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
#define SW1         12
#define SW2         A10


String        zeitanzeige, datumanzeige;
String        menu_item [12];
int           rotary_event_1 = 0, rotary_event_2 = 0, SW1_event = 0, SW2_event = 0;
int           uhr_minuten, uhr_stunden, uhr_sekunden, uhr_tag, uhr_monat, uhr_jahr, uhr_art; // Uhr
unsigned long currentTime, lastTime, letzteUhrZeit, stoppuhr_wert = 0, stoppuhr_start = 0;
boolean       encA, encB, encC, encD;
boolean       lastAB = false, lastCD = false, stoppuhr_stopp = true;
int           counter, selected_menu, selected_menu_item;

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
  pinMode       (SW1, INPUT);
  pinMode       (SW2, INPUT);

  

  currentTime = millis();
  lastTime = currentTime; 
  letzteUhrZeit = currentTime; 
  
  Serial.begin(9600); 

  LED(7,7,1000);

  menu_item [0] = "PlaySnake";
  menu_item [1] = "Stopp+Uhr";
  menu_item [2] = "Boxmaster";
  menu_item [3] = "Zirkel";
  menu_item [4] = "Ballsport";
  menu_item [5] = "Rundezeit";
  menu_item [6] = "Wertung";
  menu_item [7] = "Tennis";
  menu_item [8] = "PlaySnake";
  menu_item [9] = "Stopp+Uhr";
  menu_item [10] = "Boxmaster";
  menu_item [11] = "Zirkel";
  menu_item [12] = "Ballsport";
    
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
  delay(500);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  counter = 0;
//  rotary_event_1 = -1;
  selected_menu = 1;
  selected_menu_item = 1;
  setSyncProvider(RTC.get); 
  RESET_UHR();

}

void loop() {
  //delay(2500);
  if (selected_menu == 0) MENU();
  if (selected_menu == 1) STOPP_und_UHR();
  if (selected_menu == 2) STOPP_und_UHR();
  if (selected_menu == 3) STOPP_und_UHR();
  if (selected_menu == 4) STOPP_und_UHR();
  if (selected_menu == 5) STOPP_und_UHR();
  if (selected_menu == 6) STOPP_und_UHR();
  if (selected_menu == 7) STOPP_und_UHR();
  if (selected_menu == 8) STOPP_und_UHR();

  ROTARY();
}

void STOPP_und_UHR(){
  
  if (uhr_art == 0) UHR();
  if (uhr_art == 1) STOPPUHR();
  
  SW_PUSH();          
  if (SW1_event == 2){      // SW1 lang gedrückt?  
    SW1_event = 0;                
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    selected_menu = 0;      // gehe zu Menu
    rotary_event_1 = 1;     // Item + 1 und refresh
    selected_menu_item = 0; // Menu Item 1 (0+1)
  }
}

void STOPPUHR(){
  UHR_ZEIT();  
  STOPPUHR_ZEIT();
  
  if (SW2_event == 1){      // SW2 kurz gedrückt?  
  SW2_event = 0;   
  uhr_art = 0;
  RESET_UHR();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
}

// stoppuhr_wert = 0, stoppuhr_start

void STOPPUHR_ZEIT() {
  currentTime = millis();
  if(currentTime >= (letzteUhrZeit + 100)){
            stoppuhr_wert = stoppuhr_start - millis();
        matrix.setTextSize(1);   
        matrix.setCursor(5, 22);   
        matrix.print(stoppuhr_wert);   
      if (stoppuhr_stopp){

      }else{ 
    
      }    
    }
    
  if (SW1_event > 1){      // SW1 gedrückt?  
    SW1_event = 0;                
    if (stoppuhr_stopp){
      stoppuhr_stopp = false;
      stoppuhr_start = millis();
    }else{
      stoppuhr_stopp = true;
      stoppuhr_start = 0;   
    }
  }    
    
}

void UHR() {
  UHR_ZEIT();
  UHR_DATUM();
  if (SW2_event == 1){      // SW2 kurz gedrückt?  
  SW2_event = 0;  
  uhr_art = 1;
  RESET_UHR();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
}


void SW_PUSH(){
  if (digitalRead(SW1)){
    currentTime = millis();
    while (digitalRead(SW1)){}
    if ((millis()- currentTime) < 600) SW1_event = 1; else SW1_event = 2;}
  if (digitalRead(SW2)){
    currentTime = millis();
    while (digitalRead(SW2)){}
    if ((millis()- currentTime) < 600) SW2_event = 1; else SW2_event = 2;}
}

void MENU(){
    if (rotary_event_1 < 0){
      selected_menu_item = selected_menu_item - 1;
      if (selected_menu_item  == 0) selected_menu_item = 8;
      MENU_pfeile();
      MENU_ITEMS();
      rotary_event_1 = 0;
      }
    if (rotary_event_1 > 0){
      selected_menu_item = selected_menu_item + 1;
      if (selected_menu_item  == 9) selected_menu_item = 1;
      MENU_pfeile();
      MENU_ITEMS();
      rotary_event_1 = 0;
      }    
      
  SW_PUSH();          
  if (SW1_event > 0){      // SW1 gedrückt?  
    SW1_event = 0;                
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    selected_menu = selected_menu_item;
    RESET_UHR();
  }      
}

void MENU_ITEMS(){
  int i;
  i = selected_menu_item;
  matrix.fillRect(8, 1, 53, 8, matrix.Color333(0, 0, 0));  
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.setCursor(8, 1);  
  matrix.println(menu_item [i-1]);
  matrix.fillRect(8, 9, 53, 8, matrix.Color333(0, 0, 0));  
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.setCursor(8, 9);   
  matrix.println(menu_item [i]);  
  matrix.fillRect(8, 17, 53, 8, matrix.Color333(0, 0, 0)); 
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.setCursor(8, 17); 
  matrix.println(menu_item [i+1]);  
  matrix.fillRect(8, 25, 53, 8, matrix.Color333(0, 0, 0)); 
  matrix.setCursor(8, 25); 
  matrix.println(menu_item [i+2]);   
}

void MENU_pfeile(){
  matrix.setTextSize(1);   
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.setCursor(1, 1);  
  matrix.println(">");
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.setCursor(1, 9);  
  matrix.println(">");
  matrix.drawPixel(2, 10, matrix.Color333(0,7,0));
  matrix.drawPixel(2, 11, matrix.Color333(0,7,0));
  matrix.drawPixel(2, 12, matrix.Color333(0,7,0));
  matrix.drawPixel(2, 13, matrix.Color333(0,7,0));
  matrix.drawPixel(2, 14, matrix.Color333(0,7,0));
  matrix.drawPixel(3, 11, matrix.Color333(0,7,0));
  matrix.drawPixel(3, 12, matrix.Color333(0,7,0));
  matrix.drawPixel(3, 13, matrix.Color333(0,7,0));
  matrix.drawPixel(4, 12, matrix.Color333(0,7,0));
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.setCursor(1, 17);  
  matrix.println(">");
  matrix.setCursor(1, 25);  
  matrix.println(">");  
}




void UHR_DATUM() {
  currentTime = millis();
  if(currentTime >= (letzteUhrZeit + 1000)){

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
  }
}

void UHR_ZEIT() {
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
    if ((!encA) && (lastAB)){if (encB){rotary_event_1 = rotary_event_1 + 1;}else{rotary_event_1 = rotary_event_1 - 1;}}
    lastAB = encA;
    encC = digitalRead(PinC);
    encD = digitalRead(PinD);
    if ((!encC) && (lastCD)){if (encD){rotary_event_2 = rotary_event_2 + 1;}else{rotary_event_2 = rotary_event_2 - 1;}}
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

void RESET_UHR(){
  uhr_minuten = 0;
  uhr_stunden = 0;
  uhr_sekunden = 0;
  uhr_tag = 0;
  uhr_monat = 0;
  uhr_jahr = 0;
}
