#define LED_PUZZLE_SIZE 4

enum Colour
{
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PURPLE,
  CYAN,
  WHITE,
  BLACK
};

extern Colour lights[LED_PUZZLE_SIZE];

uint32_t get_pixel_colour(Colour colour);
void colour_puzzle_logic();
