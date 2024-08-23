#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#include <stdint.h>

#define SET_RANDOM(min, max) ((min) + rand() % ((max) - (min) + 1))

#define STRAT_X 10
#define START_Y 10
#define MAX_X 30
#define MAX_Y 30
#define START_SNAKE_SIZE 2
#define START_TIME 1000000
#define TIME_STEP 100000
#define MAX_SIZE 10

#define RESET_COLOR "\033[0m"

enum colors_t
{
  RED = 1,
  GREEN = 2,
  YELLOW = 3,
  WHITE = 4,
};

enum game_modes_t
{
  SINGLE_PLAYER = 1,
  TWO_PLAYERS = 2,
  AI = 3,
};

enum controls_t
{
  KEY_UP = 119,    // w
  KEY_RIGHT = 100, // d
  KEY_DOWN = 115,  // s
  KEY_LEFT = 97,   // a

  KEY_UP_2 = 72,    // СЃС‚СЂРµР»РєР° РІРІРµСЂС…
  KEY_RIGHT_2 = 77, // СЃС‚СЂРµР»РєР° РІРїСЂР°РІРѕ
  KEY_DOWN_2 = 80,  // СЃС‚СЂРµР»РєР° РІРЅРёР·
  KEY_LEFT_2 = 75,  // СЃС‚СЂРµР»РєР° РІР»РµРІРѕ

  KEY_STOP_GAMGE = 27,
  KEY_PAUSE_GAMGE = 112,
};

enum direction_t
{
  LEFT,
  RIGHT,
  UP,
  DOWN
};

typedef struct tail_t
{
  uint8_t x;
  uint8_t y;
} tail_t;

typedef struct snake_t
{
  uint8_t x;
  uint8_t y;
  struct tail_t *tail;
  size_t tsize;
  size_t max_size;
  enum colors_t color;
  enum direction_t direction;
  enum controls_t controls;
} snake_t;

typedef struct food_t
{
  uint8_t x;
  uint8_t y;
  enum colors_t color;
} food_t;

typedef struct game_config_t
{
  enum game_modes_t game_mode;
  enum colors_t snake_color;
  enum colors_t food_color;
} game_config_t;

#endif /* TEMP_FUNCTIONS_H */