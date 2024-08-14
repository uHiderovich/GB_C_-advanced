#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_X 15
#define MAX_Y 15

enum controls_t {
  KEY_LEFT = 97,
  KEY_RIGHT = 100,
  KEY_UP = 119,
  KEY_DOWN = 115,
  KEY_STOP_GAMGE = 27,
  KEY_PAUSE_GAMGE = 0,
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

int setRandom(uint8_t min, uint8_t max) {
  return min + rand() % (max - min + 1);
}

food_t initFood(snake_t snake, uint8_t max_x, uint8_t max_y) {
  food_t food;

  while (1) {
    food.x = setRandom(0, max_x);
    food.y = setRandom(0, max_y);

    if (snake.x != food.x && snake.y != food.y) {
      break;
    }
  }

  return food;
}

snake_t initSnake(uint8_t x, uint8_t y, size_t tsize, size_t max_size) {
  snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = tsize;
  snake.max_size = max_size;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);
  snake.direction = LEFT;

  for (int i = 0; i < tsize; ++i) {
    snake.tail[i].x = x + i + 1;
    snake.tail[i].y = y;
  }

  return snake;
}


void addTail(snake_t* snake) {
  if (snake->tsize < snake->max_size) {
    snake->tsize += 1;
  }
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

  for (int i = 0; i < snake.tsize; ++i) {
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


void printExit(snake_t snake) {
  printf("Game Over!\n");
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

snake_t move(snake_t snake) {
  for (int i = snake.tsize - 1; i > 0; i--) {
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

  if (snake.x < 0) {
    snake.x = MAX_X - 1;
  }
  else if (snake.x >= MAX_X) {
    snake.x = 0;
  }
  else if (snake.y < 0) {
    snake.y = MAX_Y - 1;
  }
  else if (snake.y >= MAX_Y) {
    snake.y = 0;
  }

  return snake;
}

int main() {
  uint8_t keyDirection = 0;
  uint8_t level = 1;

  snake_t snake = initSnake(10, 5, 2, 10);
  food_t food = initFood(snake, MAX_X, MAX_Y);

  printGameField(snake, food);

  while (keyDirection != KEY_STOP_GAMGE) {
    snake = move(snake);
    sleep(1);
    system("cls");

    if (snake.x == food.x && snake.y == food.y) {
      addTail(&snake);
      food = initFood(snake, MAX_X, MAX_Y);
      level++;
    }

    printf("Level: %d\n", level);

    printGameField(snake, food);

    if (kbhit()) {
      keyDirection = getch();
      setDirection(&snake, keyDirection);
    }
  }

  return 0;
}