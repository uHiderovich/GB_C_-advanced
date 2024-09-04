#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>

#define GET_RANDOM(max) (rand() % (max))

#define GET_MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX_X 60
#define MAX_Y 30
#define START_TRAILER_SIZE 1
#define SLEEP 500000
#define MAX_TRAILTER_SIZE 10
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
  purpose->x = GET_RANDOM(MAX_X);
  purpose->y = GET_RANDOM(MAX_Y);
}

Purpose *initPurposes() {
  Purpose *purposes = (Purpose *)malloc(sizeof(Purpose) * PURPOSES_COUNT);

  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    purposes[i].x = GET_RANDOM(MAX_X);
    purposes[i].y = GET_RANDOM(MAX_Y);
    purposes[i].enable = 1;
  }

  return purposes;
}

Drone *initDrones() {
  Drone *drones = (Drone *)malloc(sizeof(Drone) * DRONES_COUNT);

  for (size_t i = 0; i < DRONES_COUNT; i++) {
    Drone drone;
    drone.x = GET_RANDOM(MAX_X);
    drone.y = GET_RANDOM(MAX_Y);
    drone.tsize = START_TRAILER_SIZE;
    drone.trailer = (Trailer *)malloc(sizeof(Trailer) * MAX_TRAILTER_SIZE);
    drone.direction = LEFT;
    drone.enable = 1;

    for (size_t j = 0; j < drone.tsize; j++) {
      drone.trailer[j].x = (drone.x) + j + 1;
      drone.trailer[j].y = drone.y;
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
  return drone.tsize == MAX_TRAILTER_SIZE;
}

void checkIntersectPurpose(Drone *drone, Purpose *purposes) {
  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    if (drone->x == purposes[i].x && drone->y == purposes[i].y) {
      drone->tsize++;
      purposes[i].enable = 0;
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

void move(Drone *drone) {
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

void clear(Drone *drones, Purpose *purposes) {
  free(purposes);

  for (size_t i = 0; i < DRONES_COUNT; i++) {
    free(drones[i].trailer);
  }

  free(drones);
}

void autoDroneDirection(Drone *drone, Purpose purpose) {
  if (drone->direction == LEFT || drone->direction == RIGHT) {
    if (drone->x == purpose.x) {
      drone->direction = drone->y > purpose.y ? UP : DOWN;
    }
    return;
  }

  if (drone->direction == UP || drone->direction == DOWN) {
    if (drone->y == purpose.y) {
      drone->direction = drone->x > purpose.x ? LEFT : RIGHT;
    }
    return;
  }

  // if ((drone->direction == LEFT || drone->direction == RIGHT) && drone->y == purpose.y) {
  //   return;
  // }

  // if ((drone->direction == LEFT || drone->direction == RIGHT) && drone->y != purpose.y) {
  //   if (drone->x == purpose.x) {
  //     drone->direction = UP;
  //   }
  //   return;
  // }

  // if ((drone->direction == UP || drone->direction == DOWN) && drone->x == purpose.x) {
  //   return;
  // }

  // if ((drone->direction == UP || drone->direction == DOWN) && drone->x != purpose.x) {
  //   if (drone->y == purpose.y) {
  //     drone->direction = LEFT;
  //   }
  //   return;
  // }
}

int distance(const Drone drone, Purpose purpose) { // РІС‹С‡РёСЃР»СЏРµС‚ РєРѕР»РёС‡РµСЃС‚РІРѕ С…РѕРґРѕРІ РґРѕ РµРґС‹
  return (abs(drone.x - purpose.x) + abs(drone.y - purpose.y));
}

int getImmediatePurposePointer(const Drone drone, Purpose *purposes) {
  int pointer = 0;
  // РёС‰РµРј Р±Р»РёР¶Р°Р№С€СѓСЋ РµРґСѓ
  for (int i = 1; i < PURPOSES_COUNT; i++) {
    pointer = (distance(drone, purposes[i]) < distance(drone, purposes[pointer])) ? i : pointer;
  }

  // if (!purposes[pointer].enable) {
  //   pointer = getImmediatePurposePointer(drone, purposes);
  // }

  return pointer;
}

void autoChangeDirection(Drone *drone, Purpose *purposes)
{
  int pointer = getImmediatePurposePointer(*drone, purposes);

  if ((drone->direction == RIGHT || drone->direction == LEFT) && (drone->y != purposes[pointer].y)) {
    // РіРѕСЂРёР·РѕРЅС‚Р°Р»СЊРЅРѕРµ РґРІРёР¶РµРЅРёРµ
    drone->direction = (purposes[pointer].y > drone->y) ? DOWN : UP;
  }
  else if ((drone->direction == DOWN || drone->direction == UP) && (drone->x != purposes[pointer].x)) {
    // РІРµСЂС‚РёРєР°Р»СЊРЅРѕРµ РґРІРёР¶РµРЅРёРµ
    drone->direction = (purposes[pointer].x > drone->x) ? RIGHT : LEFT;
  }
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
      autoChangeDirection(&drones[i], purposes);
      checkIntersectPurpose(&drones[i], purposes);
      move(&drones[i]);
    }

    checkCrushedDrone(drones);

    if (isAllCrused = isAllDronesCrushed(drones)) {
      break;
    }

    usleep(SLEEP);
    system("cls");

    printWorkArea(drones, purposes);
  }

  if (isAllCrused) {
    printf("All drones are crushed! =(\n");
  }

  clear(drones, purposes);

  // printf("Mission completed!\n");
}


int main() {
  start();

  return 0;
}