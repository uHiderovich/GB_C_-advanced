#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
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

enum colors_t {
  RED = 1,
  GREEN = 2,
  YELLOW = 3,
  WHITE = 4,
};

enum game_modes_t {
  SINGLE_PLAYER = 1,
  TWO_PLAYERS = 2,
  AI = 3,
};

enum controls_t {
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

enum direction_t {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

typedef struct tail_t {
  uint8_t x;
  uint8_t y;
} tail_t;

typedef struct snake_t {
  uint8_t x;
  uint8_t y;
  uint8_t tsize;
  enum colors_t color;
  enum direction_t direction;
  enum controls_t controls;
  struct tail_t* tail;
} snake_t;

typedef struct food_t {
  uint8_t x;
  uint8_t y;
  enum colors_t color;
} food_t;

typedef struct game_config_t {
  enum game_modes_t game_mode;
  enum colors_t snake_color;
  enum colors_t food_color;
} game_config_t;

int isTwoPlayers(game_config_t game_config) {
  return game_config.game_mode == TWO_PLAYERS || game_config.game_mode == AI;
}

int isCoorectFoodPosition(snake_t* snake, food_t* food) {
  for (size_t i = 0; i < snake->tsize; i++) {
    if (snake->tail[i].x == food->x && snake->tail[i].y == food->y) {
      return 0;
    }
  }

  return snake->x != food->x && snake->y != food->y;
}

void updateFoodPosition(snake_t* snake, food_t* food, uint8_t max_x, uint8_t max_y) {
  while (1) {
    food->x = SET_RANDOM(0, max_x);
    food->y = SET_RANDOM(0, max_y);

    if (isCoorectFoodPosition(snake, food)) {
      break;
    }
  }
}

const char* getColor(enum colors_t color) {
  switch (color) {
  case RED:
    return "\033[1;31m";
    break;
  case GREEN:
    return "\033[1;32m";
    break;
  case YELLOW:
    return "\033[1;33m";
    break;
  default:
    return "\033[1;37m";
    break;
  }
}

food_t initFood(snake_t snake, uint8_t max_x, uint8_t max_y, game_config_t game_config) {
  food_t food;
  food.color = game_config.food_color;

  updateFoodPosition(&snake, &food, max_x, max_y);

  return food;
}

snake_t initSnake(uint8_t x, uint8_t y, game_config_t game_config) {
  snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = START_SNAKE_SIZE;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);
  snake.direction = LEFT;
  snake.color = game_config.snake_color;

  for (uint8_t i = 0; i < snake.tsize; ++i) {
    snake.tail[i].x = x + i + 1;
    snake.tail[i].y = y;
  }

  return snake;
}

void addTail(snake_t* snake) {
  snake->tsize++;
}

void addSnakeOnField(snake_t snake, char headSymbol, char field[][MAX_Y]) {
  field[snake.x][snake.y] = headSymbol;
  for (size_t i = 0; i < snake.tsize; ++i) {
    field[snake.tail[i].x][snake.tail[i].y] = '*';
  }
}

// @**
void printGameField(snake_t snake, snake_t snake2, food_t food, game_config_t game_config) {
  char game_field[MAX_X][MAX_Y];

  for (int i = 0; i < MAX_X; ++i) {
    for (int j = 0; j < MAX_Y; ++j) {
      game_field[i][j] = ' ';
    }
  }

  addSnakeOnField(snake, '@', game_field);
  if (isTwoPlayers(game_config)) addSnakeOnField(snake2, '&', game_field);

  game_field[food.x][food.y] = '$';

  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      printf("%c", game_field[i][j]);

      // РћРєСЂР°СЃ Р·РјРµР№РєРё Рё РµРґС‹
      // if (game_field[i][j] == '*' || game_field[i][j] == '@') {
      //   printf("%s%c%s", getColor(game_config.snake_color), game_field[i][j], RESET_COLOR);
      // }
      // else if (game_field[i][j] == '$') {
      //   printf("%s%c%s", getColor(game_config.food_color), game_field[i][j], RESET_COLOR);
      // }
      // else {
      //   printf("%c", game_field[i][j]);
      // }
    }
    printf("\n");
  }
}

void setDirectionPlayer1(snake_t* snake, uint8_t keyDirection) {
  if (
    (keyDirection == KEY_UP && snake->direction == DOWN) ||
    (keyDirection == KEY_DOWN && snake->direction == UP) ||
    (keyDirection == KEY_LEFT && snake->direction == RIGHT) ||
    (keyDirection == KEY_RIGHT && snake->direction == LEFT)
    ) {
    return;
  }

  switch (keyDirection) {
  case KEY_UP:
    snake->direction = UP;
    break;
  case KEY_LEFT:
    snake->direction = LEFT;
    break;
  case KEY_DOWN:
    snake->direction = DOWN;
    break;
  case KEY_RIGHT:
    snake->direction = RIGHT;
    break;
  default:
    break;
  }
}

void setDirectionPlayer2(snake_t* snake, uint8_t keyDirection) {
  if (
    (keyDirection == KEY_UP_2 && snake->direction == DOWN) ||
    (keyDirection == KEY_DOWN_2 && snake->direction == UP) ||
    (keyDirection == KEY_LEFT_2 && snake->direction == RIGHT) ||
    (keyDirection == KEY_RIGHT_2 && snake->direction == LEFT)
    ) {
    return;
  }

  switch (keyDirection) {
  case KEY_UP_2:
    snake->direction = UP;
    break;
  case KEY_LEFT_2:
    snake->direction = LEFT;
    break;
  case KEY_DOWN_2:
    snake->direction = DOWN;
    break;
  case KEY_RIGHT_2:
    snake->direction = RIGHT;
    break;
  default:
    break;
  }
}

void checkFieldBoundaries(snake_t* snake) {
  if (snake->x < 0) {
    snake->x = MAX_X - 1;
  }
  else if (snake->x >= MAX_X) {
    snake->x = 0;
  }
  else if (snake->y < 0) {
    snake->y = MAX_Y - 1;
  }
  else if (snake->y >= MAX_Y) {
    snake->y = 0;
  }
}

void checkPause(_Bool* isPaused, uint8_t keyDirection) {
  if (keyDirection == KEY_PAUSE_GAMGE) {
    *isPaused = !(*isPaused);
  }
}

int checkIntersectFood(snake_t* snake, food_t* food) {
  if (snake->x == food->x && snake->y == food->y) {
    addTail(snake);
    updateFoodPosition(snake, food, MAX_X, MAX_Y);
    return 1;
  }
  return 0;
}

int isNewLevel(snake_t* snake, snake_t* snake2, food_t* food, game_config_t game_config) {
  return checkIntersectFood(snake, food) || (isTwoPlayers(game_config) && checkIntersectFood(snake2, food));
}

int isCrush(snake_t snake) {
  for (size_t i = 0; i < snake.tsize; i++) {
    if (snake.tail[i].x == snake.x && snake.tail[i].y == snake.y) {
      return 1;
    }
  }

  return 0;
}

int isGameOver(snake_t snake, snake_t snake2) {
  return snake.tsize == MAX_SIZE || snake2.tsize == MAX_SIZE || isCrush(snake) || isCrush(snake2);
};

snake_t move(snake_t* snake) {
  for (size_t i = snake->tsize - 1; i > 0; i--) {
    snake->tail[i] = snake->tail[i - 1];
  }

  snake->tail[0].x = snake->x;
  snake->tail[0].y = snake->y;

  switch (snake->direction) {
  case LEFT:
    snake->x = snake->x - 1;
    break;
  case RIGHT:
    snake->x = snake->x + 1;
    break;
  case UP:
    snake->y = snake->y - 1;
    break;
  case DOWN:
    snake->y = snake->y + 1;
    break;
  default:
    break;
  }

  checkFieldBoundaries(snake);
}

void play(game_config_t game_config) {
  uint8_t keyDirection = 0;
  uint8_t level = 1;
  _Bool isPaused = 0;

  snake_t snake = initSnake(MAX_X / 3, MAX_Y / 3, game_config);
  snake_t snake2 = initSnake(MAX_X / 2, MAX_Y / 2, game_config);

  food_t food = initFood(snake, MAX_X, MAX_Y, game_config);

  printGameField(snake, snake2, food, game_config);

  while (keyDirection != KEY_STOP_GAMGE) {
    if (kbhit()) {
      keyDirection = getch();
      setDirectionPlayer1(&snake, keyDirection);
      if (isTwoPlayers(game_config)) setDirectionPlayer2(&snake2, keyDirection);
      checkPause(&isPaused, keyDirection);
    }

    printf("Level: %d\n", level);

    if (isPaused) {
      printf("Pause..\n");
    }
    else {
      move(&snake);
      if (isTwoPlayers(game_config)) move(&snake2);
    }

    if (isGameOver(snake, snake2)) {
      break;
    }

    usleep(START_TIME - (TIME_STEP * level));
    system("cls");

    if (isNewLevel(&snake, &snake2, &food, game_config)) {
      level++;
    }

    printGameField(snake, snake2, food, game_config);
  }

  printf("Game Over!\n");
}

int selectGameMode() {
  printf("Select the game mode and press Enter:\n");
  printf("1: Quick game\n");
  printf("2: Two players\n");
  printf("3: Game with AI\n");

  int value = 0;

  while (value > 2 || value < 1) {
    scanf("%d", &value);

    if (value > 2 || value < 1) {
      printf("Wrong the game mode! Try again!\n");
    }
  }

  return value;
}

int selectColor() {
  printf("1: Red\n");
  printf("2: Yellow\n");
  printf("3: Green\n");

  int value = 0;

  while (value > 3 || value < 1) {
    scanf("%d", &value);

    if (value > 3 || value < 1) {
      printf("Wrong color variant! Try again!\n");
    }
  }

  return value;
}

void startMenu(game_config_t* game_config) {
  printf("Welcome to Snake Game!\n\n");

  game_config->game_mode = selectGameMode();

  printf("Choose the color of the snakes and press Enter\n");
  game_config->snake_color = selectColor();

  printf("Choose the color of the food and press Enter\n");
  game_config->food_color = selectColor();
}

int main() {
  game_config_t game_config;

  startMenu(&game_config);

  play(game_config);

  return 0;
}