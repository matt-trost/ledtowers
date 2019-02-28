
// Driving the LED Tower with FastLED library.
// Definitely not production code, still under development and must be improved.
// probably need fundamental changes in the display code, and we need to figure
// out why the LEDs are too dim...

// built off of the Cylon FastLED example.


#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 42


/////////////////////////////////////////////////////////////
#define AUDIO_LEFT_IN A0
#define POT_IN A1

#define NUMPIXELS 42 // Number of LEDs in strip
#define NUMLEVELS 14  // number of levels in the LED tower.
//#define MAX_AUDIO_VALUE 850  // determined by the op amp output under the conditions we tested.

#define AUDIO_UNDRIVEN_VALUE 550  // the avg number we read when audio is disconnected.
#define MIN_DEAD_RADIUS 50  // base dead zone of 500-600, centered at 550.
#define MAX_DEAD_RADIUS 150  //
/////////////////////////////////////////////////////////////

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 4
#define CLOCK_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(115200);
	Serial.println("resetting");
	LEDS.addLeds<APA102,DATA_PIN,CLOCK_PIN, BGR, DATA_RATE_MHZ(24)>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}

void loop() {

  // read the potentiometer 0-1000. If 1000, high sensitivity, thus smaller changes in volume have proportionally larger effects on tower height.
  // implemented via increasing or decreasing dead zone range.
  int sensitivity = analogRead(POT_IN);
  //Serial.print("sensitivity=");
  //Serial.println(sensitivity);

  // at zero sensitivity, dead zone range is 500-600.
  // at max sensitivity, dead zone range is 400-700.
  // Driving from an iPhone SE, max volume looks good when dead zone is 460-640.

  int meta_radius = MAX_DEAD_RADIUS - MIN_DEAD_RADIUS;
  double sensitivity_proportion = (1023.0 - double(sensitivity)) / 1023.0;
  int dead_radius = MIN_DEAD_RADIUS + (meta_radius * sensitivity_proportion);

  int val = analogRead(AUDIO_LEFT_IN);
  // val should be between ~280 and ~820
  // val under no driving fluctuates btwn 500 and 600.
  // so we ignore those values.
  int dead_lower_bound = AUDIO_UNDRIVEN_VALUE - dead_radius;
  int dead_upper_bound = AUDIO_UNDRIVEN_VALUE + dead_radius;
  if (val > dead_upper_bound && val < dead_lower_bound) { return; }
  int diff;
  if (val >= dead_upper_bound) {
    diff = val - dead_upper_bound;
  }
  else {
    diff = dead_lower_bound - val;
  }

  //Serial.println(val);
  //Serial.println(diff);
//  int max_audio_value = 2*dead_radius + dead_upper_bound;
//  int range = max_audio_value - dead_upper_bound;
  int range = (1.5 * dead_radius);
  int n = int(range / NUMLEVELS);
  int level = int(diff/n);
  int total_num_leds = level * 3;
  //Serial.println(total_num_leds);
  //Serial.println();
  //CHSV color = CHSV(random8(), 255, 255);
  //CRGB color = CRGB(255, 255, 255);
  CRGB color = CRGB::White;
  for (int i = 0; i < NUMPIXELS; ++i) {
    if (i < total_num_leds) {
      leds[i] = color;
    } else {
      leds[i] = CRGB(0,0,0);
    }
  }
  FastLED.show();
  
}
