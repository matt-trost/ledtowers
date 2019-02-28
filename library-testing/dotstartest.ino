
#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 42 // Number of LEDs in strip
#define DATAPIN    4
#define CLOCKPIN   5

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);


void setup() {
  Serial.begin(115200);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  for (int i = 0; i < NUMPIXELS; ++i) {
    strip.setPixelColor(i, 255, 255, 255);
  }
  strip.show();
  Serial.println("one");
}

void loop() {
//  for (int i = 0; i < NUMPIXELS; ++i) {
//    strip.setPixelColor(i, 255, 255, 255);
//  }
//  strip.show();
}
