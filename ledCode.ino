#include <FastLED.h>

// Declaring Pins
#define LED_PIN     7
#define NUM_LEDS    60

// Declaring Global Variable
int depth;
int groundDist = 914.4;       // 3 ft
String incomingString;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
}

void loop() {
  int flag = 0;
  if(Serial.available()) {
    incomingString = Serial.readString();
    flag = 1;
  }
  
  if(flag == 1){
    String value = "";
    int commacount = 0;
    delay(200);
    //Serial.println("Incoming string: " + incomingString);
    for(int i = 0; i < incomingString.length(); i++){
      //delay(500);
      //Serial.println("In loop: " + i);
      char current = incomingString[i];
      //delay(250);
      if(current == ','){
        //delay(100);
        commacount = commacount + 1;
        //delay(100);
      }
      //delay(100);
      if(commacount == 3){
        //delay(100);
        commacount = 0;
        //delay(100);
        break;
      }
      //delay(100);
      if(commacount == 2){
        if(current == ','){
          continue;
        }
        //delay(100);
        value.concat(current);
        //delay(100);
        //Serial.println("Current string: " + value);
        //delay(200);
      }
    }
    flag = 0;
    depth = value.toInt();
    depth = abs(depth - groundDist);
    Serial.print("Depth: ");
    Serial.println(depth);
  }
  
  if (depth > 150 && depth < 500) {
    for (int i = 0; i < 60; i++) {
      leds[i] = CRGB(255, 255, 0);
    }
    FastLED.show();
    delay(1000);
    for (int i = 0; i < 60; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
    delay(1000);
  }

  if (depth >= 500) {
    for (int i = 0; i < 60; i++) {
      leds[i] = CRGB(255, 0, 0);
    }
    FastLED.show();
    delay(1000);
    for (int i = 0; i < 60; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
    delay(1000);
  }

  //FastLED.show();
  
}

// No LED is under 6 inch
// Flashing yellow between 6-20 
// Flashing red is over 20
