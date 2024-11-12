#include <Arduino.h>

#define BLINK_PUZZLE_LED_INDEX 0
#define BLINK_SEQUENCE_LENGTH 6
#define BLINK_PUZZLE_FORGIVENESS 150

struct BlinkStep
{
  bool led_on;
  uint32_t start;
  uint32_t end;
};

extern BlinkStep blink_sequence[BLINK_SEQUENCE_LENGTH];
extern uint16_t sequence_step;

void blink_puzzle_logic(int delta);