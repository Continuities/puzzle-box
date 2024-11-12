#include "blink-puzzle.h"
#include "input.h"

// Blink puzzle state
BlinkStep blink_sequence[BLINK_SEQUENCE_LENGTH] = {
    {true, 0, 1500},
    {false, 1500, 3000},
    {true, 3000, 3800},
    {false, 3800, 4600},
    {true, 4600, 5400},
    {false, 5400, 6200},
};
uint32_t sequence_time = 0;
uint16_t sequence_step = 0;
bool button_in_sync = false;
bool step_completed = false;
uint16_t successful_steps = 0;

int get_sequence_step()
{
  // This is super inefficient, but the sequence is short and so it should be fine
  for (int i = 0; i < BLINK_SEQUENCE_LENGTH; i++)
  {
    BlinkStep step = blink_sequence[i];
    if (sequence_time >= step.start && sequence_time < step.end)
    {
      return i;
    }
  }
  return -1;
}

void blink_puzzle_logic(int delta)
{
  sequence_time += delta;
  int step_index = get_sequence_step();
  if (step_index < 0)
  {
    step_index = 0;
    sequence_time -= blink_sequence[BLINK_SEQUENCE_LENGTH - 1].end;
  }
  sequence_step = step_index;
  BlinkStep step = blink_sequence[sequence_step];
  bool in_sync = silver_button.pushed == step.led_on;
  if (sequence_time - step.start < BLINK_PUZZLE_FORGIVENESS)
  {
    step_completed = false;
    if (in_sync)
    {
      button_in_sync = true;
    }
  }
  else if (step.end - sequence_time < BLINK_PUZZLE_FORGIVENESS)
  {
    if (!step_completed)
    {
      if (button_in_sync)
      {
        successful_steps++;
        Serial.print("STEP SUCCESSFUL: ");
        Serial.println(successful_steps);
      }
      else
      {
        Serial.println("STEP FAILED!");
        successful_steps = 0;
      }
      step_completed = true;
    }
  }
  else if (!in_sync)
  {
    button_in_sync = false;
    successful_steps = 0;
  }

  if (successful_steps == BLINK_SEQUENCE_LENGTH)
  {
    Serial.println("BLINK PUZZLE COMPLETE!");
    // TODO
  }
}