#include "types.h"
#include "game/terrain.h"

/* 2 for HQ, 1 for the other terrains in the switch (city, airport, port, base,
 * pipe seam, silo, com tower), 0 for the rest. */
int Ov006_GetPropertyKind(int terrain) {
  int kind = 0;

  switch (terrain & 0x1f) {
    case TERRAIN_HQ:
      kind = 2;
      break;
    case TERRAIN_CITY:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_PIPE_SEAM:
    case TERRAIN_SILO:
    case TERRAIN_COM_TOWER:
      kind = 1;
      break;
  }
  return kind;
}
