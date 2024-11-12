#include "lights.h"
#include "colour-puzzle.h"
#include "blink-puzzle.h"

// leds
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel status_pixels = Adafruit_NeoPixel(NUM_STATUS_LEDS, STATUS_LED_PIN, NEO_RGB + NEO_KHZ800);

void init_lights()
{
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(BRIGHTNESS);
  pixels.show();
  status_pixels.begin();
  status_pixels.clear();
  status_pixels.setBrightness(BRIGHTNESS);
  status_pixels.show();
}

void update_lights()
{
  // LED puzzle lights
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, get_pixel_colour(lights[i]));
  }
  pixels.show();

  // Blink puzzle lights
  status_pixels.setPixelColor(BLINK_PUZZLE_LED_INDEX, blink_sequence[sequence_step].led_on ? 0xffffff : 0);
  status_pixels.show();
}