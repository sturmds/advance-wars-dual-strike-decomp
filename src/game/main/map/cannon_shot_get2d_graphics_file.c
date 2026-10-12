#include "types.h"
#include "game/map_object.h"

extern char data_02168218[];
extern char data_0216821c[];

/* Name of the file with the shot graphics for sprites, per cannon kind. */
char* CannonShot_Get2dGraphicsFile(int kind) {
  switch (kind) {
    case MAP_OBJECT_BLACK_CANNON:
    default:
      return data_02168218;
    case MAP_OBJECT_MINICANNON:
      return data_0216821c;
  }
}
