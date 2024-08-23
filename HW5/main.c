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

#define RESET_COLOR   "\033[0m"
// #define RED     "\033[1;31m"
// #define GREEN  "\033[1;32m"
// #define YELLOW  "\033[1;33m"
// #define WHITE   "\033[1;37m"

enum colors_t {
  RED = 1,
  GREEN = 2,
  YELLOW = 3,
  WHITE = 4,
};

enum controls_t {
  KEY_UP = 119, // w
  KEY_RIGHT = 100, // d
  KEY_DOWN = 115, // s
  KEY_LEFT = 97, // a

  KEY_UP_2 = 72, // СЃС‚СЂРµР»РєР° РІРІРµСЂС…
  KEY_RIGHT_2 = 77, // СЃС‚СЂРµР»РєР° РІРїСЂР°РІРѕ
  KEY_DOWN_2 = 80, // СЃС‚СЂРµР»РєР° РІРЅРёР·
  KEY_LEFT_2 = 75, // СЃС‚СЂРµР»РєР° РІР»РµРІРѕ

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
  struct tail_t* tail;
  size_t tsize;
  size_t max_size;
  char color;
  enum direction_t direction;
  enum controls_t controls;
} snake_t;

typedef struct food_t {
  uint8_t x;
  uint8_t y;
} food_t;

int isCoorectFoodPosition(snake_t snake, food_t food) {
  for (size_t i = 0; i < snake.tsize; i++) {
    if (snake.tail[i].x == food.x && snake.tail[i].y == food.y) {
      return 0;
    }
  }

  return snake.x != food.x && snake.y != food.y;
}

void updateFoodPosition(snake_t snake, food_t* food, uint8_t max_x, uint8_t max_y) {
  while (1) {
    food->x = SET_RANDOM(0, max_x);
    food->y = SET_RANDOM(0, max_y);

    if (isCoorectFoodPosition(snake, *food)) {
      break;
    }
  }
}

food_t initFood(snake_t snake, uint8_t max_x, uint8_t max_y) {
  food_t food;

  updateFoodPosition(snake, &food, max_x, max_y);

  return food;
}

snake_t initSnake(uint8_t x, uint8_t y, size_t tsize) {
  snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = tsize;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);
  snake.direction = LEFT;
  snake.color = WHITE;

  for (uint8_t i = 0; i < tsize; ++i) {
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
void printGameField(snake_t snake, snake_t snake2, food_t food, int playrsCount) {
  char matrix[MAX_X][MAX_Y];

  for (int i = 0; i < MAX_X; ++i) {
    for (int j = 0; j < MAX_Y; ++j) {
      matrix[i][j] = ' ';
    }
  }

  addSnakeOnField(snake, '@', matrix);

  if (playrsCount == 2) {
    addSnakeOnField(snake2, '&', matrix);
  }

  matrix[food.x][food.y] = '*';

  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      printf("%s%c%s", "\033[1;31m", matrix[i][j], "\033[0m");
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
    addTail(&snake);
    updateFoodPosition(*snake, &food, MAX_X, MAX_Y);
    return 1;
  }
  return 0;
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

  checkFieldBoundaries(&snake);
}


void play(int game_mode) {
  uint8_t keyDirection = 0;
  uint8_t level = 1;
  _Bool isPaused = 0;

  snake_t snake = initSnake(MAX_X / 3, MAX_Y / 3, START_SNAKE_SIZE);
  snake_t snake2;

  if (game_mode == 2) snake2 = initSnake(MAX_X / 2, MAX_Y / 2, START_SNAKE_SIZE);

  food_t food = initFood(snake, MAX_X, MAX_Y);

  printGameField(snake, snake2, food, game_mode);

  while (keyDirection != KEY_STOP_GAMGE) {
    if (kbhit()) {
      keyDirection = getch();
      setDirectionPlayer1(&snake, keyDirection);
      if (game_mode == 2) setDirectionPlayer2(&snake2, keyDirection);
      checkPause(&isPaused, keyDirection);
    }

    printf("Level: %d\n", level);

    if (isPaused) {
      printf("Pause..\n");
    }
    else {
      move(&snake);
      if (game_mode == 2) move(&snake2);
    }

    if (isGameOver(snake, snake2)) {
      break;
    }

    usleep(START_TIME - (TIME_STEP * level));
    system("cls");

    if (checkIntersectFood(&snake, &food) || (game_mode == 2 && checkIntersectFood(&snake2, &food))) {
      level++;
    }

    printGameField(snake, snake2, food, game_mode);
  }

  printf("Game Over!\n");
}

int selectGameMode() {
  printf("Select the game mode and press Enter:\n");
  printf("1: Quick game\n");
  printf("2: Two players\n");
  // printf("3: Game with AI\n");

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

void startMenu() {
  printf("Welcome to Snake Game!\n\n");

  int game_mode = selectGameMode();

  printf("Choose the color of the snakes\n");
  int snakesColor = selectColor();

  printf("Choose the color of the food\n");
  int foodColor = selectColor();

  // play(game_mode);
}

int main() {
  startMenu();

  return 0;
}