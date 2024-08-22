#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>

#define SET_RANDOM(min, max) ((min) + rand() % ((max) - (min) + 1))

#define STRAT_X 10
#define START_Y 10
#define MAX_X 15
#define MAX_Y 15
#define START_SNAKE_SIZE 2
#define START_TIME 1000000
#define TIME_STEP 100000
#define MAX_LEVEL 10

enum controls_t {
  KEY_LEFT = 97,
  KEY_RIGHT = 100,
  KEY_UP = 119,
  KEY_DOWN = 115,
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
  enum direction_t direction;
  enum controls_t controls;
} snake_t;

typedef struct food_t {
  uint8_t x;
  uint8_t y;
} food_t;

food_t initFood(snake_t snake, uint8_t max_x, uint8_t max_y) {
  food_t food;

  while (1) {
    food.x = SET_RANDOM(0, max_x);
    food.y = SET_RANDOM(0, max_y);

    if (isCoorectFoodPosition(snake, food)) {
      break;
    }
  }

  return food;
}

int isCoorectFoodPosition(snake_t snake, food_t food) {
  for (size_t i = 0; i < snake.tsize; i++) {
    if (snake.tail[i].x == food.x && snake.tail[i].y == food.y) {
      return 0;
    }
  }

  return snake.x != food.x && snake.y != food.y;
}

snake_t initSnake(uint8_t x, uint8_t y, size_t tsize) {
  snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = tsize;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);
  snake.direction = LEFT;

  for (size_t i = 0; i < tsize; ++i) {
    snake.tail[i].x = x + i + 1;
    snake.tail[i].y = y;
  }

  return snake;
}

void addTail(snake_t* snake) {
  snake->tsize += 1;
}

// @**
void printGameField(snake_t snake, food_t food) {
  char matrix[MAX_X][MAX_Y];

  for (int i = 0; i < MAX_X; ++i) {
    for (int j = 0; j < MAX_Y; ++j) {
      matrix[i][j] = ' ';
    }
  }

  matrix[snake.x][snake.y] = '@';

  for (size_t i = 0; i < snake.tsize; ++i) {
    matrix[snake.tail[i].x][snake.tail[i].y] = '*';
  }

  matrix[food.x][food.y] = '*';

  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

void setDirection(snake_t* snake, uint8_t keyDirection) {
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

int checkPause(_Bool isPaused, uint8_t keyDirection) {
  if (keyDirection == KEY_PAUSE_GAMGE) {
    isPaused = !isPaused;
  }

  return isPaused;
}

int isCrush(snake_t snake) {
  for (size_t i = 0; i < snake.tsize; i++) {
    if (snake.tail[i].x == snake.x && snake.tail[i].y == snake.y) {
      return 1;
    }
  }

  return 0;
}

snake_t move(snake_t snake) {
  for (size_t i = snake.tsize - 1; i > 0; i--) {
    snake.tail[i] = snake.tail[i - 1];
  }

  snake.tail[0].x = snake.x;
  snake.tail[0].y = snake.y;

  switch (snake.direction) {
  case LEFT:
    snake.x = snake.x - 1;
    break;
  case RIGHT:
    snake.x = snake.x + 1;
    break;
  case UP:
    snake.y = snake.y - 1;
    break;
  case DOWN:
    snake.y = snake.y + 1;
    break;
  default:
    break;
  }

  checkFieldBoundaries(&snake);

  return snake;
}

int main() {
  uint8_t keyDirection = 0;
  uint8_t level = 1;
  _Bool isPaused = 0;

  snake_t snake = initSnake(STRAT_X, START_Y, START_SNAKE_SIZE);
  food_t food = initFood(snake, MAX_X, MAX_Y);

  printGameField(snake, food);

  while (keyDirection != KEY_STOP_GAMGE) {
    if (kbhit()) {
      keyDirection = getch();
      setDirection(&snake, keyDirection);
      isPaused = checkPause(isPaused, keyDirection);
    }

    printf("Level: %d\n", level);

    if (isPaused) {
      printf("Pause..\n");
    }
    else {
      snake = move(snake);
    }

    if (snake.tsize == MAX_LEVEL || isCrush(snake)) {
      break;
    }

    usleep(START_TIME - (TIME_STEP * level));
    system("cls");

    if (snake.x == food.x && snake.y == food.y) {
      addTail(&snake);
      food = initFood(snake, MAX_X, MAX_Y);
      level++;
    }

    printGameField(snake, food);
  }

  printf("Game Over!\n");

  return 0;
}