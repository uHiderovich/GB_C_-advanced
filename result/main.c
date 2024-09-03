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
#define SLEEP 500000
#define MAX_SIZE 10
#define PURPOSES_COUNT 5
#define DRONES_COUNT 1

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
  _Bool enable;
} Drone;

typedef struct Purpose {
  uint8_t x;
  uint8_t y;
  _Bool enable;
} Purpose;

void updatePurposePosition(Purpose *purpose) {
  purpose->x = SET_RANDOM(0, MAX_X);
  purpose->y = SET_RANDOM(0, MAX_Y);
}

Purpose *initPurposes() {
  Purpose *purposes = (Purpose *)malloc(sizeof(Purpose) * PURPOSES_COUNT);

  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    Purpose purpose;
    updatePurposePosition(&purpose);
    purpose.enable = 1;
    purposes[i] = purpose;
  }

  return purposes;
}

Drone *initDrones() {
  Drone *drones = (Drone *)malloc(sizeof(Drone) * DRONES_COUNT);

  for (size_t i = 0; i < DRONES_COUNT; i++) {
    Drone drone;
    drone.x = SET_RANDOM(0, MAX_X);
    drone.y = SET_RANDOM(0, MAX_Y);
    drone.tsize = START_TRAILER_SIZE;
    drone.trailer = (Trailer *)malloc(sizeof(Trailer) * MAX_SIZE);
    drone.direction = LEFT;
    drone.enable = 1;

    for (uint8_t i = 0; i < drone.tsize; ++i) {
      drone.trailer[i].x = drone.x + i + 1;
      drone.trailer[i].y = drone.y;
    }

    drones[i] = drone;
  }

  return drones;
}

void addDronesOnArea(Drone *drones, char area[][MAX_Y]) {
  for (size_t i = 0; i < DRONES_COUNT; i++) {
    area[drones[i].x][drones[i].y] = '@';

    for (size_t j = 0; j < drones[i].tsize; j++) {
      area[drones[i].trailer[j].x][drones[i].trailer[j].y] = '*';
    }

  }
}

void addPurposeOnArea(Purpose *purposes, char area[][MAX_Y]) {
  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    if (purposes[i].enable) {
      area[purposes[i].x][purposes[i].y] = '$';
    }
  }
}

void printWorkArea(Drone *drones, Purpose *purposes) {
  char work_area[MAX_X][MAX_Y];

  for (int i = 0; i < MAX_X; ++i) {
    for (int j = 0; j < MAX_Y; ++j) {
      work_area[i][j] = ' ';
    }
  }

  addPurposeOnArea(purposes, work_area);

  addDronesOnArea(drones, work_area);

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

int isMissionCompleted(Drone drone) {
  return drone.tsize == MAX_SIZE;
}

void checkIntersectPurpose(Drone *drones, Purpose *purposes) {
  for (size_t i = 0; i < DRONES_COUNT; i++) {
    for (size_t j = 0; j < PURPOSES_COUNT; j++) {
      if (drones[i].x == purposes[j].x && drones[i].y == purposes[j].y) {
        drones[i].tsize++;
        purposes[j].enable = 0;
      }
    }
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

void checkCrushedDrone(Drone *drones) {
  for (size_t i = 0; i < DRONES_COUNT; i++) {
    if (drones[i].enable && isCrush(drones[i])) {
      drones[i].enable = 0;
      printf("Drone %d is crushed!\n", (int)i + 1);
    }
  }
}

_Bool isAllDronesCrushed(Drone *drones) {
  for (size_t i = 0; i < DRONES_COUNT; i++) {
    if (drones[i].enable) {
      return 0;
    }
  }

  return 1;
}

void move(Drone drone) {
  for (size_t i = drone.tsize - 1; i > 0; i--) {
    drone.trailer[i] = drone.trailer[i - 1];
  }

  drone.trailer[0].x = drone.x;
  drone.trailer[0].y = drone.y;

  switch (drone.direction) {
  case LEFT:
    drone.x = GET_MAX(drone.x - 1, 0);
    break;
  case RIGHT:
    drone.x = GET_MIN(drone.x + 1, MAX_X);
    break;
  case UP:
    drone.y = GET_MAX(drone.y - 1, 0);
    break;
  case DOWN:
    drone.y = GET_MIN(drone.y + 1, MAX_Y);
    break;
  default:
    break;
  }

  // checkFieldBoundaries(drone);
}

void start() {
  uint8_t keyDirection = 0;
  _Bool isAllCrused = 0;

  Drone *drones = initDrones();
  Purpose *purposes = initPurposes();

  printWorkArea(drones, purposes);

  while (keyDirection != KEY_STOP_GAMGE) {
    // if (kbhit()) {
    //   keyDirection = getch();
    //   setDirection(&drone, keyDirection);
    // }

    for (size_t i = 0; i < DRONES_COUNT; i++) {
      move(drones[i]);
    }

    checkIntersectPurpose(drones, purposes);

    checkCrushedDrone(drones);

    if (isAllCrused = isAllDronesCrushed(drones)) {
      break;
    }

    usleep(SLEEP);
    system("cls");

    printWorkArea(drones, purposes);
  }

  if (isAllCrused) {
    printf("All drones are crushed! =(");
  }

  // printf("Mission completed!\n");
}


int main() {
  start();

  return 0;
}