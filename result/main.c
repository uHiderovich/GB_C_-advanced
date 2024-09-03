#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>

#define SET_RANDOM(min, max) ((min) + rand() % ((max) - (min) + 1))

#define GET_MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_MIN(a, b) ((a) < (b) ? (a) : (b))

#define STRAT_X 10
#define START_Y 10
#define MAX_X 30
#define MAX_Y 30
#define START_TRAILER_SIZE 1
#define SLEEP 700000
#define MAX_SIZE 10
#define PURPOSE_COUNT 5

enum Controls {
  KEY_UP = 119,    // w
  KEY_RIGHT = 100, // d
  KEY_DOWN = 115,  // s
  KEY_LEFT = 97,   // a

  KEY_STOP_GAMGE = 27,
  KEY_PAUSE_GAMGE = 112,
};

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

typedef struct Trailer {
  uint8_t x;
  uint8_t y;
} Trailer;

typedef struct Drone {
  uint8_t x;
  uint8_t y;
  uint8_t tsize;
  enum Direction direction;
  enum Controls controls;
  struct Trailer *trailer;
} Drone;

typedef struct Purpose {
  uint8_t x;
  uint8_t y;
} Purpose;

Purpose *initPurposes() {
  Purpose *purposes = (Purpose *)malloc(sizeof(Purpose) * PURPOSE_COUNT);

  for (size_t i = 0; i < PURPOSE_COUNT; i++) {
    Purpose purpose;
    purpose.x = SET_RANDOM(0, MAX_X);
    purpose.y = SET_RANDOM(0, MAX_Y);
    purposes[i] = purpose;
  }

  return purposes;
}

Drone initDrone(uint8_t x, uint8_t y) {
  Drone drone;
  drone.x = x;
  drone.y = y;
  drone.tsize = START_TRAILER_SIZE;
  drone.trailer = (Trailer *)malloc(sizeof(Trailer) * MAX_SIZE);
  drone.direction = LEFT;

  for (uint8_t i = 0; i < drone.tsize; ++i) {
    drone.trailer[i].x = x + i + 1;
    drone.trailer[i].y = y;
  }

  return drone;
}

void addTrailer(Drone *drone) {
  drone->tsize++;
}

void addDroneOnArea(Drone drone, char area[][MAX_Y]) {
  area[drone.x][drone.y] = '@';
  for (size_t i = 0; i < drone.tsize; ++i) {
    area[drone.trailer[i].x][drone.trailer[i].y] = '*';
  }
}

void printWorkArea(Drone drone, Purpose *purposes) {
  char work_area[MAX_X][MAX_Y];

  for (int i = 0; i < MAX_X; ++i) {
    for (int j = 0; j < MAX_Y; ++j) {
      work_area[i][j] = ' ';
    }
  }

  for (size_t i = 0; i < PURPOSE_COUNT; i++) {
    work_area[purposes[i].x][purposes[i].y] = '$';
  }

  addDroneOnArea(drone, work_area);

  for (int j = 0; j < MAX_Y; ++j) {
    for (int i = 0; i < MAX_X; ++i) {
      printf("%c", work_area[i][j]);
    }
    printf("\n");
  }
}

void setDirection(Drone *drone, uint8_t keyDirection) {
  if (
    (keyDirection == KEY_UP && drone->direction == DOWN) ||
    (keyDirection == KEY_DOWN && drone->direction == UP) ||
    (keyDirection == KEY_LEFT && drone->direction == RIGHT) ||
    (keyDirection == KEY_RIGHT && drone->direction == LEFT)
    ) {
    return;
  }

  switch (keyDirection) {
  case KEY_UP:
    drone->direction = UP;
    break;
  case KEY_LEFT:
    drone->direction = LEFT;
    break;
  case KEY_DOWN:
    drone->direction = DOWN;
    break;
  case KEY_RIGHT:
    drone->direction = RIGHT;
    break;
  default:
    break;
  }
}

void checkFieldBoundaries(Drone *drone) {
  if (drone->x <= 0) {
    drone->x = 0;
  }
  else if (drone->x >= MAX_X) {
    drone->x = MAX_X;
  }
  else if (drone->y <= 0) {
    drone->y = 0;
  }
  else if (drone->y >= MAX_Y) {
    drone->y = MAX_Y;
  }
}

void checkPause(_Bool *isPaused, uint8_t keyDirection) {
  if (keyDirection == KEY_PAUSE_GAMGE) {
    *isPaused = !(*isPaused);
  }
}

int isCrush(Drone drone) {
  for (size_t i = 0; i < drone.tsize; i++) {
    if (drone.trailer[i].x == drone.x && drone.trailer[i].y == drone.y) {
      return 1;
    }
  }

  return 0;
}

int isMissionCompleted(Drone drone) {
  return drone.tsize == MAX_SIZE || isCrush(drone);
};

Drone move(Drone *drone) {
  for (size_t i = drone->tsize - 1; i > 0; i--) {
    drone->trailer[i] = drone->trailer[i - 1];
  }

  drone->trailer[0].x = drone->x;
  drone->trailer[0].y = drone->y;

  switch (drone->direction) {
  case LEFT:
    drone->x = GET_MAX(drone->x - 1, 0);
    break;
  case RIGHT:
    drone->x = GET_MIN(drone->x + 1, MAX_X);
    break;
  case UP:
    drone->y = GET_MAX(drone->y - 1, 0);
    break;
  case DOWN:
    drone->y = GET_MIN(drone->y + 1, MAX_Y);
    break;
  default:
    break;
  }

  // checkFieldBoundaries(drone);
}

void start() {
  uint8_t keyDirection = 0;

  Drone drone = initDrone(MAX_X / 2, MAX_Y / 2);
  Purpose *purposes = initPurposes();

  printWorkArea(drone, purposes);

  while (keyDirection != KEY_STOP_GAMGE) {
    if (kbhit()) {
      keyDirection = getch();
      setDirection(&drone, keyDirection);

    }

    move(&drone);

    if (isMissionCompleted(drone)) {
      break;
    }

    usleep(SLEEP);
    system("cls");

    printWorkArea(drone, purposes);
  }

  printf("Mission completed!\n");
}


int main() {
  start();

  return 0;
}