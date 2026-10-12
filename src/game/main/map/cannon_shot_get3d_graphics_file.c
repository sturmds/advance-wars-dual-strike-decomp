#include "types.h"
#include "game/map_object.h"

extern char data_02168210[];
extern char data_02168214[];

/* Name of the file with the shot graphics for the 3D engine, per cannon
 * kind. */
char* CannonShot_Get3dGraphicsFile(int kind) {
  switch (kind) {
    case MAP_OBJECT_BLACK_CANNON:
    default:
      return data_02168210;
    case MAP_OBJECT_MINICANNON:
      return data_02168214;
  }
}
