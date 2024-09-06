#include "drone.h"

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
    drone.autoControl = 1;

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

  drone->enable = 1;
}

void checkIntersectPurpose(Drone *drone, Purpose *purposes) {
  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    if (drone->x == purposes[i].x && drone->y == purposes[i].y) {
      drone->tsize++;
      purposes[i].enable = 0;
    }
  }
}

int isCrushDrone(Drone drone) {
  for (size_t i = 0; i < drone.tsize; i++) {
    if (drone.trailer[i].x == drone.x && drone.trailer[i].y == drone.y) {
      return 1;
    }
  }

  if (drone.x < 0 || drone.y < 0 || drone.y >(MAX_Y - 1) || drone.x >(MAX_X - 1)) {
    return 1;
  }

  return 0;
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
    drone->x = (drone->x - 1);
    // if (drone->x == 0) {
    //   drone->direction = (drone->y == 0 ? DOWN : UP);
    // }
    break;
  case RIGHT:
    drone->x = (drone->x + 1);
    // if (drone->x == (MAX_X - 1)) {
    //   drone->direction = (drone->y == (MAX_Y - 1) ? UP : DOWN);
    // }
    break;
  case UP:
    drone->y = (drone->y - 1);
    // if (drone->y == 0) {
    //   drone->direction = (drone->x == 0 ? RIGHT : LEFT);
    // }
    break;
  case DOWN:
    drone->y = (drone->y + 1);
    // if (drone->y == (MAX_Y - 1)) {
    //   drone->direction = (drone->x == (MAX_X - 1) ? LEFT : RIGHT);
    // }
    break;
  default:
    break;
  }
}

void clear(Drone *drones, Purpose *purposes) {
  free(purposes);

  for (size_t i = 0; i < DRONES_COUNT; i++) {
    free(drones[i].trailer);
  }

  free(drones);
}

int distance(const Drone drone, Purpose purpose) {
  return (abs(drone.x - purpose.x) + abs(drone.y - purpose.y));
}

int getImmediatePurposePointer(const Drone drone, Purpose *purposes) {
  int pointer = 0;

  for (int i = 1; i < PURPOSES_COUNT; i++) {
    if (purposes[pointer].enable) {
      pointer = (distance(drone, purposes[i]) < distance(drone, purposes[pointer])) && purposes[i].enable ? i : pointer;
    }
    else {
      pointer++;
    }
  }

  return pointer;
}

void autoChangeDirection(Drone *drone, Purpose *purposes) {
  int pointer = getImmediatePurposePointer(*drone, purposes);

  if ((drone->direction == RIGHT || drone->direction == LEFT) && (drone->y != purposes[pointer].y)) {
    drone->direction = (purposes[pointer].y > drone->y) ? DOWN : UP;
  }
  else if ((drone->direction == DOWN || drone->direction == UP) && (drone->x != purposes[pointer].x)) {
    drone->direction = (purposes[pointer].x > drone->x) ? RIGHT : LEFT;
  }
}

int isAllPurposeDisabled(Purpose *purposes) {
  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    if (purposes[i].enable) {
      return 0;
    }
  }

  return 1;
}

void refreshPurposes(Purpose *purposes) {
  for (size_t i = 0; i < PURPOSES_COUNT; i++) {
    purposes[i].enable = 1;
    updatePurposePosition(&purposes[i]);
  }
}
