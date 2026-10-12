#include "types.h"
#include "game/ov003_unit.h"
#include "game/terrain.h"

void Ov003_DefeatTeam(int team); /* called with the team that lost its HQ */

#define MAP data_ov003_02364a88

/* Gives the property at map index tile to team (1 to 5) by changing its tile
 * code to that team's version. info holds the old terrain (bits 0-4) and old
 * owner (bits 5-7). A captured HQ turns into a city of the new team, and the
 * old owner is passed to Ov003_DefeatTeam. */
void Ov003_CaptureProperty(int tile, int info, int team) {
  switch (info & 31) {
    case TERRAIN_CITY:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1c7;
          break;
        case 2:
          MAP->cells[tile] = 0x1cc;
          break;
        case 3:
          MAP->cells[tile] = 0x1d1;
          break;
        case 4:
          MAP->cells[tile] = 0x1d6;
          break;
        case 5:
          MAP->cells[tile] = 0x1e2;
          break;
      }
      break;
    case TERRAIN_BASE:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1c6;
          break;
        case 2:
          MAP->cells[tile] = 0x1cb;
          break;
        case 3:
          MAP->cells[tile] = 0x1d0;
          break;
        case 4:
          MAP->cells[tile] = 0x1d5;
          break;
        case 5:
          MAP->cells[tile] = 0x1e1;
          break;
      }
      break;
    case TERRAIN_AIRPORT:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1c8;
          break;
        case 2:
          MAP->cells[tile] = 0x1cd;
          break;
        case 3:
          MAP->cells[tile] = 0x1d2;
          break;
        case 4:
          MAP->cells[tile] = 0x1d7;
          break;
        case 5:
          MAP->cells[tile] = 0x1e3;
          break;
      }
      break;
    case TERRAIN_PORT:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1c9;
          break;
        case 2:
          MAP->cells[tile] = 0x1ce;
          break;
        case 3:
          MAP->cells[tile] = 0x1d3;
          break;
        case 4:
          MAP->cells[tile] = 0x1d8;
          break;
        case 5:
          MAP->cells[tile] = 0x1e4;
          break;
      }
      break;
    case TERRAIN_COM_TOWER:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1ba;
          break;
        case 2:
          MAP->cells[tile] = 0x1bb;
          break;
        case 3:
          MAP->cells[tile] = 0x1bc;
          break;
        case 4:
          MAP->cells[tile] = 0x1bd;
          break;
        case 5:
          MAP->cells[tile] = 0x1be;
          break;
      }
      break;
    case TERRAIN_LAB:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1da;
          break;
        case 2:
          MAP->cells[tile] = 0x1db;
          break;
        case 3:
          MAP->cells[tile] = 0x1dc;
          break;
        case 4:
          MAP->cells[tile] = 0x1dd;
          break;
        case 5:
          MAP->cells[tile] = 0x1de;
          break;
      }
      break;
    case TERRAIN_HQ:
      switch (team) {
        case 1:
          MAP->cells[tile] = 0x1c7;
          break;
        case 2:
          MAP->cells[tile] = 0x1cc;
          break;
        case 3:
          MAP->cells[tile] = 0x1d1;
          break;
        case 4:
          MAP->cells[tile] = 0x1d6;
          break;
        case 5:
          MAP->cells[tile] = 0x1e2;
          break;
      }
      Ov003_DefeatTeam((info >> 5) & 7);
      break;
  }
}
