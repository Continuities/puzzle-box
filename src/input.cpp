#include "input.h"

// inputs
ButtonState square_button = {SQUARE_BUTTON_PIN, false, false};
ButtonState silver_button = {SILVER_BUTTON_PIN, false, false};
SwitchState red_switch = {RED_SWITCH_PIN, false};
SwitchState green_switch = {GREEN_SWITCH_PIN, false};
SwitchState blue_switch = {BLUE_SWITCH_PIN, false};

void init_inputs()
{
  pinMode(square_button.pin, INPUT_PULLUP);
  pinMode(silver_button.pin, INPUT_PULLUP);
  pinMode(red_switch.pin, INPUT_PULLUP);
  pinMode(green_switch.pin, INPUT_PULLUP);
  pinMode(blue_switch.pin, INPUT_PULLUP);
}

void read_button(ButtonState *button_state)
{
  bool pushed = digitalRead(button_state->pin) == LOW;
  button_state->pushed_this_frame = pushed && !button_state->pushed;
  button_state->pushed = pushed;
}

void read_buttons()
{
  read_button(&square_button);
  read_button(&silver_button);
}

void read_switch(SwitchState *switch_state)
{
  switch_state->switched_on = digitalRead(switch_state->pin) == LOW;
}

void read_switches()
{
  read_switch(&red_switch);
  read_switch(&green_switch);
  read_switch(&blue_switch);
}

void print_buttons()
{
  if (square_button.pushed_this_frame)
  {
    Serial.println("square_button pushed");
  }
  if (silver_button.pushed_this_frame)
  {
    Serial.println("silver_button pushed");
  }
}

void debug_buttons()
{
  Serial.print("square: ");
  Serial.print(digitalRead(SQUARE_BUTTON_PIN));
  Serial.print(" silver: ");
  Serial.println(digitalRead(SILVER_BUTTON_PIN));
}

void debug_switches()
{
  Serial.print("RGB: ");
  Serial.print(red_switch.switched_on);
  Serial.print(green_switch.switched_on);
  Serial.print(blue_switch.switched_on);
  Serial.println("");
}