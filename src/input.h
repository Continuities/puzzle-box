#include <Arduino.h>

#define KNOB_STEPS 20
#define KNOB_DEGREES_PER_CLICK 360 / KNOB_STEPS

#define SQUARE_BUTTON_PIN 12
#define SILVER_BUTTON_PIN 11
#define RED_SWITCH_PIN 10
#define GREEN_SWITCH_PIN 9
#define BLUE_SWITCH_PIN 6

#define FRONT_KNOB_CLK_PIN 16
#define FRONT_KNOB_DT_PIN 17

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

struct KnobState
{
  uint32_t clk_pin;
  uint32_t dt_pin;
  int a_val;
  int b_val;
  int angle;
};

extern ButtonState square_button;
extern ButtonState silver_button;
extern SwitchState red_switch;
extern SwitchState green_switch;
extern SwitchState blue_switch;
extern KnobState front_knob;

void init_inputs();
void read_buttons();
void read_switches();
void read_knobs();
void debug_knobs();