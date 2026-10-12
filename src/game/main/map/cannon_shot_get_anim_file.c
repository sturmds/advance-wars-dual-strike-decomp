#include "types.h"
#include "game/map_object.h"

extern u8 data_0213fe0c[];
extern u8 data_021400d8[];

/* Sprite animation file of the shot, per cannon kind. */
u8* CannonShot_GetAnimFile(int kind) {
  switch (kind) {
    case MAP_OBJECT_BLACK_CANNON:
    default:
      return data_0213fe0c;
    case MAP_OBJECT_MINICANNON:
      return data_021400d8;
  }
}
