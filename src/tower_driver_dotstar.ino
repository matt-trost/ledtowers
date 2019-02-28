
// Driving the LED Tower with FastLED library.
// Definitely not production code, still under development and must be improved.
// probably need fundamental changes in the display code, and we need to figure
// out why the LEDs are too dim...

// Built off of the Adafruit DotStar example code.

#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 42 // Number of LEDs in strip
#define NUMLEVELS 14  // number of levels in the LED tower.
//#define MAX_AUDIO_VALUE 850  // determined by the op amp output under the conditions we tested.

#define AUDIO_UNDRIVEN_VALUE 550  // the avg number we read when audio is disconnected.
#define MIN_DEAD_RADIUS 50  // base dead zone of 500-600, centered at 550.
#define MAX_DEAD_RADIUS 150  //

// Audio 
#define AUDIO_LEFT_IN A0 // Audio LEFT
#define POT_IN A1
#define DATAPIN    4
#define CLOCKPIN   5

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  Serial.begin(112500);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

uint32_t color = 0x080008;
int count = 0;

void loop() {

  // read the potentiometer 0-1000. If 1000, high sensitivity, thus smaller changes in volume have proportionally larger effects on tower height.
  // implemented via increasing or decreasing dead zone range.
  int sensitivity = analogRead(POT_IN);
  Serial.print("sensitivity=");
  Serial.println(sensitivity);

  // at zero sensitivity, dead zone range is 500-600.
  // at max sensitivity, dead zone range is 400-700.
  // On iphone SE, max volume looks good when dead zone is 460-640.

  int meta_radius = MAX_DEAD_RADIUS - MIN_DEAD_RADIUS;
  double sensitivity_proportion = (1023.0 - double(sensitivity)) / 1023.0;
  int dead_radius = MIN_DEAD_RADIUS + (meta_radius * sensitivity_proportion);

  int val = analogRead(AUDIO_LEFT_IN);
  // val should be between ~280 and ~820
  // val under no driving fluctuates btwn 500 and 600.
  // so we ignore those values.
  int dead_lower_bound = AUDIO_UNDRIVEN_VALUE - dead_radius;
  int dead_upper_bound = AUDIO_UNDRIVEN_VALUE + dead_radius;
  //if (val > dead_upper_bound && val < dead_lower_bound) { return; }
  //while (val < dead_upper_bound && val > dead_lower_bound) { val = analogRead(AUDIO_LEFT_IN); }
  int diff;
  if (val >= dead_upper_bound) {
    diff = val - dead_upper_bound;
  }
  else if (val <= dead_lower_bound) {
    diff = dead_lower_bound - val;
  }
  else {
    diff = 0;  // in the dead zone, turn all LEDs off!
  }

  Serial.print("val=");
  Serial.println(val);
  Serial.print("diff=");
  Serial.println(diff);
//  int max_audio_value = 2*dead_radius + dead_upper_bound;
//  int range = max_audio_value - dead_upper_bound;
  int range = (1.5 * dead_radius);
  int n = int(range / NUMLEVELS);
  int level = int(diff/n);
  int total_num_leds = level * 3;
  Serial.print("total leds=");
  Serial.println(total_num_leds);
  Serial.println();

//  strip.setPixelColor(39, 255, 255, 255);
//  strip.setPixelColor(40, 255, 255, 255);
//  strip.setPixelColor(41, 255, 255, 255);
//  strip.show();

//    for (int i = 0; i < NUMPIXELS; ++i) {
//      strip.setPixelColor(i, 255, 255, 255);
//    }
//    strip.show();

  if ( count == 0) {
    color = ((random(0, 256) << 16) |  (random(0, 256) << 8) | random(0, 256));
    count = 100;
  }
  count--;
  for (int i = 0; i < NUMPIXELS; ++i) {
    if (i < total_num_leds) {
      strip.setPixelColor(i, color);
      //strip.setPixelColor(i, 0x42, 0xdf, 0xff); // strip.setPixelColor(i, 0x42, 0xdf, 0xf4);
      //strip.setPixelColor(i, 255, 255, 255);
    } else {
      strip.setPixelColor(i, 0x000000);
    }
  }
  strip.show();
  
}
