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


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// DISPLAY_ON() // Display einschalten
// DISPLAY_ON() // Display ausschalten

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



  digitalWrite(BLUE_1, LOW);
  delay(500); 
  digitalWrite(BLUE_1, HIGH);
  digitalWrite(RED_1, LOW);
  delay(500); 
  digitalWrite(RED_1, HIGH);
  digitalWrite(GREEN_1, LOW);
  delay(500); 
  digitalWrite(GREEN_1, HIGH);
  
  DISPLAY_ON();
  matrix.begin();

  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, matrix.Color333(7, 0, 0));
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(40, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);

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

  // whew!
}

void loop() {
  // Do nothing -- image doesn't change
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
