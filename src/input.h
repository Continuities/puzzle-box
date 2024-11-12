#include <Arduino.h>

#define SQUARE_BUTTON_PIN 12
#define SILVER_BUTTON_PIN 11
#define RED_SWITCH_PIN 10
#define GREEN_SWITCH_PIN 9
#define BLUE_SWITCH_PIN 6

struct ButtonState
{
  uint32_t pin;
  bool pushed;
  bool pushed_this_frame;
};

struct SwitchState
{
  uint32_t pin;
  bool switched_on;
};

extern ButtonState square_button;
extern ButtonState silver_button;
extern SwitchState red_switch;
extern SwitchState green_switch;
extern SwitchState blue_switch;

void init_inputs();
void read_buttons();
void read_switches();