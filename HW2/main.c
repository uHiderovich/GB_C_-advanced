#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_X 15
#define MAX_Y 15

enum controls_t {
  KEY_LEFT = 97,
  KEY_RIGHT = 100,
  KEY_UP = 119,
  KEY_DOWN = 115,
  KEY_STOP_GAMGE = 27,
};

enum direction_t {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

typedef struct tail_t {
  int x;
  int y;
} tail_t;

typedef struct snake_t {
  int x;
  int y;
  struct tail_t* tail;
  size_t tsize;
  enum direction_t direction;
  enum direction_t forbiddenDirection;
  enum controls_t controls;
} snake_t;

struct snake_t initSnake(int x, int y, size_t tsize) {
  struct snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = tsize;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);
  snake.direction = LEFT;
  snake.forbiddenDirection = RIGHT;

  for (int i = 0; i < tsize; ++i) {
    snake.tail[i].x = x + i + 1;
    snake.tail[i].y = y;
  }

  return snake;
}

// @**
void printSnake(struct snake_t snake) {
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

  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

void setDirection(snake_t* snake, int keyDirection) {
  enum direction_t newDirection;

  switch (keyDirection) {
  case KEY_UP:
    newDirection = UP;
    break;
  case KEY_LEFT:
    newDirection = LEFT;
    break;
  case KEY_DOWN:
    newDirection = DOWN;
    break;
  case KEY_RIGHT:
    newDirection = RIGHT;
    break;
  default:
    break;
  }

  if (newDirection != snake->forbiddenDirection) {
    snake->direction = newDirection;
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
    snake.forbiddenDirection = RIGHT;
    break;
  case RIGHT:
    snake.x = snake.x + 1;
    snake.forbiddenDirection = LEFT;
    break;
  case UP:
    snake.y = snake.y - 1;
    snake.forbiddenDirection = DOWN;
    break;
  case DOWN:
    snake.y = snake.y + 1;
    snake.forbiddenDirection = UP;
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
  struct snake_t snake = initSnake(10, 5, 2);
  printSnake(snake);
  int keyDirection = 0;

  while (keyDirection != KEY_STOP_GAMGE) {
    snake = move(snake);
    sleep(1);
    system("cls");
    printSnake(snake);

    if (kbhit()) {
      keyDirection = getch();
      setDirection(&snake, keyDirection);
    }
  }

  return 0;
}