#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "colour-puzzle.h"
#include "input.h"

// LED puzzle state
int light_index = 0;
Colour lights_target[LED_PUZZLE_SIZE] = {RED, YELLOW, GREEN, PURPLE}; // TODO: Make this random on startup
Colour lights[LED_PUZZLE_SIZE] = {BLACK, BLACK, BLACK, BLACK};

Colour get_switch_colour()
{
  if (red_switch.switched_on && green_switch.switched_on && blue_switch.switched_on)
  {
    return WHITE;
  }
  if (red_switch.switched_on && green_switch.switched_on)
  {
    return YELLOW;
  }
  if (red_switch.switched_on && blue_switch.switched_on)
  {
    return PURPLE;
  }
  if (green_switch.switched_on && blue_switch.switched_on)
  {
    return CYAN;
  }
  if (red_switch.switched_on)
  {
    return RED;
  }
  if (green_switch.switched_on)
  {
    return GREEN;
  }
  if (blue_switch.switched_on)
  {
    return BLUE;
  }
  return BLACK;
}

uint32_t get_pixel_colour(Colour colour)
{
  switch (colour)
  {
  case RED:
    return Adafruit_NeoPixel::Color(255, 0, 0);
  case GREEN:
    return Adafruit_NeoPixel::Color(0, 255, 0);
  case BLUE:
    return Adafruit_NeoPixel::Color(0, 0, 255);
  case YELLOW:
    return Adafruit_NeoPixel::Color(255, 255, 0);
  case PURPLE:
    return Adafruit_NeoPixel::Color(255, 0, 255);
  case CYAN:
    return Adafruit_NeoPixel::Color(0, 255, 255);
  case WHITE:
    return Adafruit_NeoPixel::Color(255, 255, 255);
  case BLACK:
  default:
    return Adafruit_NeoPixel::Color(0, 0, 0);
  }
}

bool is_colour_puzzle_solved()
{
  for (int i = 0; i < LED_PUZZLE_SIZE; i++)
  {
    if (lights[i] != lights_target[i])
    {
      return false;
    }
  }
  return true;
}

void colour_puzzle_logic()
{
  // When the square button is pushed, set the light colours based on RGB switches
  if (!square_button.pushed_this_frame)
  {
    return;
  }
  Colour colour = get_switch_colour();
  Serial.print("SETTING ");
  Serial.println(colour);
  lights[LED_PUZZLE_SIZE - light_index - 1] = colour;
  light_index = (light_index + 1) % LED_PUZZLE_SIZE;

  if (is_colour_puzzle_solved())
  {
    Serial.println("LIGHT PUZZLE SOLVED");
    // TODO
  }
}