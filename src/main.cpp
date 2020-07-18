#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIXEL_COUNT 40
#define PIXEL_PIN 6
#define BUTTON 4

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int mode = 0;
int state = 0;
int buttonState = 1;

void setup() {
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void displayColor(uint32_t color) {
  for(int i = 0; i < PIXEL_COUNT; i++) { 
    strip.setPixelColor(i, color);
 }
}

void chenille(int step) {
	for(uint8_t i=0; i<PIXEL_COUNT; i++) {
		uint8_t d = (step + PIXEL_COUNT - i) % (PIXEL_COUNT);
		strip.setPixelColor(i, 255 >> d, 255 >> d, 255 >> d);
	}
}

void loop() {
  int value = analogRead(A0);
  if (mode == 0) {
    uint8_t a = pow(2,value/128);
    displayColor(strip.Color(a,a,a));
  } 
  if (mode == 1) {
    displayColor(Wheel(value/8));
  } 
  if (mode == 2) {
		uint8_t a = pow(2,abs(map(state, 0, 1023, -8, 8)));
    state += value/16 + 1;
    if ( state >= 1024) {
      state -= 1024;
    }
    displayColor(strip.Color(a,a,a));
  }
  if (mode == 3) {
    state += value/16 + 1;
    if ( state >= 1024) {
      state -= 1024;
    }
    chenille(map(state, 0, 1024, 0, 39));
  }
  if (mode == 4) {
    displayColor(strip.Color(0,0,0));
  }


  strip.show();
  delay(10);
  if (digitalRead(BUTTON) == 0 && buttonState) {
    mode++;
    if (mode == 5) {
      mode = 0;
    }
  }
  buttonState = digitalRead(BUTTON);
}