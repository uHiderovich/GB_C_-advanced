#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "drone.h"

void start(ProgramConfig config) {
  _Bool isStandBy = 0;
  uint8_t keyDirection = 0;
  int8_t manualOperationDroneNumber = -1;

  uint16_t msec = 0, trigger = 5000;
  clock_t startTime;

  Drone *drones = initDrones(config);
  Purpose *purposes = initPurposes();

  printWorkArea(drones, purposes, config);

  while (keyDirection != KEY_STOP) {
    if (kbhit()) {
      keyDirection = getch();

      if (keyDirection == KEY_HAND_CONTROL) {
        if (manualOperationDroneNumber > -1) {
          drones[manualOperationDroneNumber - 1].autoControl = 1;
          manualOperationDroneNumber = -1;
        }
        else {
          printf("Enter the number of the drone for manual operation: ");
          scanf("%d", &manualOperationDroneNumber);

          if (manualOperationDroneNumber >= 1 && manualOperationDroneNumber <= config.dronesCount) {
            drones[manualOperationDroneNumber - 1].autoControl = 0;
          }
          else {
            printf("Incorrect drone number!\n");
          }
        }
      }

      if (manualOperationDroneNumber > -1) {
        setDirection(&drones[manualOperationDroneNumber - 1], keyDirection);
        drones[manualOperationDroneNumber - 1].enable = 1;
      }
    }

    if (isAllPurposeDisabled(purposes)) {
      if (!isStandBy) {
        isStandBy = 1;
        startTime = clock();
      }

      clock_t diff = clock() - startTime;
      msec = diff * 1000 / CLOCKS_PER_SEC;

      if (msec >= trigger) {
        refreshPurposes(purposes);
        isStandBy = 0;
      }
    }

    for (size_t i = 0; i < config.dronesCount; i++) {
      if (drones[i].enable) {
        if (!isStandBy && drones[i].autoControl) autoChangeDirection(&drones[i], purposes);

        if (!isStandBy) checkIntersectPurpose(&drones[i], purposes);

        if (isCrushDrone(drones[i])) drones[i].enable = 0;

        move(&drones[i], isStandBy);
      }
      else {
        printf("Drone %d is crushed!\n", i + 1);
      }
    }

    usleep(DELAY);
    system("cls");

    printWorkArea(drones, purposes, config);
  }

  clear(drones, purposes, config);
}

int main() {
  ProgramConfig config;
  config.dronesCount = 0;

  while (1)
  {
    printf("Enter the number of drones in the area:\n");
    scanf("%d", &config.dronesCount);

    if (config.dronesCount < 1 || config.dronesCount > 5) {
      printf("Incorrect number of drones!\n");
      usleep(MENU_DELAY);
      continue;
    }
    else {
      break;
    }
  }

  start(config);

  return 0;
}
