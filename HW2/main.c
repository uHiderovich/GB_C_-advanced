#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_X 15
#define MAX_Y 15

typedef struct tail_t {
  int x;
  int y;
} tail_t;

typedef struct snake_t {
  int x;
  int y;
  struct tail_t* tail;
  size_t tsize;
} snake_t;

struct snake_t initSnake(int x, int y, size_t tsize) {
  struct snake_t snake;
  snake.x = x;
  snake.y = y;
  snake.tsize = tsize;
  snake.tail = (tail_t*)malloc(sizeof(tail_t) * 100);

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

snake_t moveLeft(snake_t snake) {
  for (int i = snake.tsize - 1; i > 0; i--) {
    snake.tail[i] = snake.tail[i - 1];
  }

  snake.tail[0].x = snake.x;
  snake.tail[0].y = snake.y;

  snake.x = snake.x - 1;

  if (snake.x < 0) {
    snake.x = MAX_X - 1;
  }

  return snake;
}

int main() {
  struct snake_t snake = initSnake(10, 5, 2);
  printSnake(snake);

  while (1) {
    snake = moveLeft(snake);
    sleep(1);
    system("cls");
    printSnake(snake);
  }

  return 0;
}