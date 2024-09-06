#ifndef DRONE_H
#define DRONE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define GET_RANDOM(max) (rand() % (max))

#define GET_MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX_X 30
#define MAX_Y 30
#define START_TRAILER_SIZE 1
#define DELAY 100000
#define MAX_TRAILTER_SIZE 10
#define PURPOSES_COUNT 1
#define DRONES_COUNT 1

enum Controls
{
  KEY_UP = 119,    // w
  KEY_RIGHT = 100, // d
  KEY_DOWN = 115,  // s
  KEY_LEFT = 97,   // a

  KEY_STOP = 27,          // esc
  KEY_HAND_CONTROL = 112, // p
};

enum Direction
{
  LEFT,
  RIGHT,
  UP,
  DOWN
};

typedef struct Trailer
{
  uint8_t x;
  uint8_t y;
} Trailer;

typedef struct Drone
{
  uint8_t x;
  uint8_t y;
  uint8_t tsize;
  enum Direction direction;
  enum Controls controls;
  struct Trailer *trailer;
  _Bool enable;
  _Bool autoControl;
} Drone;

typedef struct Purpose
{
  uint8_t x;
  uint8_t y;
  _Bool enable;
} Purpose;

void updatePurposePosition(Purpose *purpose);

Purpose *initPurposes();

Drone *initDrones();

void addDronesOnArea(Drone *drones, char area[][MAX_Y]);

void addPurposeOnArea(Purpose *purposes, char area[][MAX_Y]);

void printWorkArea(Drone *drones, Purpose *purposes);

void setDirection(Drone *drone, uint8_t keyDirection);

void setDirection(Drone *drone, uint8_t keyDirection);

void setDirection(Drone *drone, uint8_t keyDirection);

void checkIntersectPurpose(Drone *drone, Purpose *purposes);

int isCrushDrone(Drone drone);

_Bool isAllDronesCrushed(Drone *drones);

void move(Drone *drone);

void clear(Drone *drones, Purpose *purposes);

void autoDroneDirection(Drone *drone, Purpose purpose);

int distance(const Drone drone, Purpose purpose);

int getImmediatePurposePointer(const Drone drone, Purpose *purposes);

void autoChangeDirection(Drone *drone, Purpose *purposes);

int isAllPurposeDisabled(Purpose *purposes);

void refreshPurposes(Purpose *purposes);

#endif /* DRONE_H */