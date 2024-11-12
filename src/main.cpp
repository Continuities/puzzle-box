#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_NeoPixel.h>
#include "input.h"
#include "rotation.h"
#include "colour-puzzle.h"
#include "blink-puzzle.h"
#include "lights.h"

long last_frame;

void setup()
{
  Serial.begin(115200);

  last_frame = millis();

  init_inputs();
  init_lights();
  rotation_setup();
}

void loop()
{
  long this_frame = millis();
  long delta = this_frame - last_frame;
  last_frame = this_frame;

  rotation_loop();
  read_buttons();
  read_switches();
  read_knobs();
  colour_puzzle_logic();
  blink_puzzle_logic(delta);
  update_lights();

  // printButtons();
  // debugButtons();
  // debugSwitches();
  // debug_knobs();
}
