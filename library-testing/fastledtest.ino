#include <FastLED.h>
#define NUM_LEDS 3

#define DATA_PIN 4
#define CLOCK_PIN 5

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<APA102,DATA_PIN,CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds,NUM_LEDS);
  LEDS.setBrightness(250);
}

void loop() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
  Serial.println("one");
}

