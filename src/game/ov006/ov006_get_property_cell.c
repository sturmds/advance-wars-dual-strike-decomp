#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Map cell codes of property tiles: five per owner (0 neutral, 1-4),
 * starting at 0x1c0, in the order of the PROP_* offsets. */
#define PROP_HQ 0
#define PROP_BASE 1
#define PROP_CITY 2
#define PROP_AIRPORT 3
#define PROP_PORT 4
#define PROPERTY_CELL(owner, prop) (0x1c0 + (owner) * 5 + (prop))
#define COM_TOWER_CELL(owner) (0x1b9 + (owner))
#define SILO_CELL 0x180

/* Returns the map cell code for a MapTiles.tiles byte that holds a
 * property, missile silo or com tower (terrain and owner), or 0 for any
 * other tile. */
int Ov006_GetPropertyCell(int tile) {
  int cell = 0;

  switch (tile) {
    case TERRAIN_CITY:
      cell = PROPERTY_CELL(0, PROP_CITY);
      break;
    case MAP_TILE(TERRAIN_CITY, 1):
      cell = PROPERTY_CELL(1, PROP_CITY);
      break;
    case MAP_TILE(TERRAIN_CITY, 2):
      cell = PROPERTY_CELL(2, PROP_CITY);
      break;
    case MAP_TILE(TERRAIN_CITY, 3):
      cell = PROPERTY_CELL(3, PROP_CITY);
      break;
    case MAP_TILE(TERRAIN_CITY, 4):
      cell = PROPERTY_CELL(4, PROP_CITY);
      break;
    case TERRAIN_HQ:
      cell = PROPERTY_CELL(0, PROP_HQ);
      break;
    case MAP_TILE(TERRAIN_HQ, 1):
      cell = PROPERTY_CELL(1, PROP_HQ);
      break;
    case MAP_TILE(TERRAIN_HQ, 2):
      cell = PROPERTY_CELL(2, PROP_HQ);
      break;
    case MAP_TILE(TERRAIN_HQ, 3):
      cell = PROPERTY_CELL(3, PROP_HQ);
      break;
    case MAP_TILE(TERRAIN_HQ, 4):
      cell = PROPERTY_CELL(4, PROP_HQ);
      break;
    case TERRAIN_AIRPORT:
      cell = PROPERTY_CELL(0, PROP_AIRPORT);
      break;
    case MAP_TILE(TERRAIN_AIRPORT, 1):
      cell = PROPERTY_CELL(1, PROP_AIRPORT);
      break;
    case MAP_TILE(TERRAIN_AIRPORT, 2):
      cell = PROPERTY_CELL(2, PROP_AIRPORT);
      break;
    case MAP_TILE(TERRAIN_AIRPORT, 3):
      cell = PROPERTY_CELL(3, PROP_AIRPORT);
      break;
    case MAP_TILE(TERRAIN_AIRPORT, 4):
      cell = PROPERTY_CELL(4, PROP_AIRPORT);
      break;
    case TERRAIN_PORT:
      cell = PROPERTY_CELL(0, PROP_PORT);
      break;
    case MAP_TILE(TERRAIN_PORT, 1):
      cell = PROPERTY_CELL(1, PROP_PORT);
      break;
    case MAP_TILE(TERRAIN_PORT, 2):
      cell = PROPERTY_CELL(2, PROP_PORT);
      break;
    case MAP_TILE(TERRAIN_PORT, 3):
      cell = PROPERTY_CELL(3, PROP_PORT);
      break;
    case MAP_TILE(TERRAIN_PORT, 4):
      cell = PROPERTY_CELL(4, PROP_PORT);
      break;
    case TERRAIN_BASE:
      cell = PROPERTY_CELL(0, PROP_BASE);
      break;
    case MAP_TILE(TERRAIN_BASE, 1):
      cell = PROPERTY_CELL(1, PROP_BASE);
      break;
    case MAP_TILE(TERRAIN_BASE, 2):
      cell = PROPERTY_CELL(2, PROP_BASE);
      break;
    case MAP_TILE(TERRAIN_BASE, 3):
      cell = PROPERTY_CELL(3, PROP_BASE);
      break;
    case MAP_TILE(TERRAIN_BASE, 4):
      cell = PROPERTY_CELL(4, PROP_BASE);
      break;
    case TERRAIN_SILO:
      cell = SILO_CELL;
      break;
    case TERRAIN_COM_TOWER:
      cell = COM_TOWER_CELL(0);
      break;
    case MAP_TILE(TERRAIN_COM_TOWER, 1):
      cell = COM_TOWER_CELL(1);
      break;
    case MAP_TILE(TERRAIN_COM_TOWER, 2):
      cell = COM_TOWER_CELL(2);
      break;
    case MAP_TILE(TERRAIN_COM_TOWER, 3):
      cell = COM_TOWER_CELL(3);
      break;
    case MAP_TILE(TERRAIN_COM_TOWER, 4):
      cell = COM_TOWER_CELL(4);
      break;
  }
  return cell;
}
