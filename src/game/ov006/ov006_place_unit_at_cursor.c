#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_ov006_0235f690.h"

void func_020c4018(void);
int Ov006_RemoveUnitAt(int withEffect, int x, int y);
int Ov006_GetUnitCodeAt(int x, int y);
int Player_CountUnits(int player);
MapUnit* Unit_Spawn(s16 x, s16 y, s16 type);
void func_ov006_0235f690(void);

#define OV6 data_ov006_0236d640
#define ARMY_UNIT_LIMIT 50 /* units one army may have on a Design Room map */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* Unit mode edit on the cursor tile for army unitArmy. With flag 0x40 set
 * (delete) it removes the unit there and returns 2. Otherwise it puts unit
 * type toolUnit there, replacing a different unit, and returns 1. It returns
 * -1 when the unit cannot move onto the tile, or when the army already has
 * 50 units and the tile holds another army's unit; 0 when nothing changed
 * (same unit already there, or still 50 units after the swap). */
int Ov006_PlaceUnitAtCursor(void) {
  int code;
  int x;
  int y;
  int old;
  int result = 0;

  MAP_STATE->currentPlayer = OV6->unitArmy;
  MAP_STATE->currentUnitBase = (MAP_STATE->currentPlayer - 1) << 6;
  code = (OV6->toolUnit & 63) | MAP_STATE->currentUnitBase;
  x = OV6->cursorX;
  y = OV6->cursorY;
  if (OV6->unk_00 & 0x40) {
    if (Ov006_RemoveUnitAt(1, x, y)) {
      func_020c4018();
      result = 2;
    }
  } else {
    old = Ov006_GetUnitCodeAt(x, y);
    if (code != old) {
      if (old != UNIT_TYPE_COUNT) {
        if (Player_CountUnits(OV6->unitArmy) >= ARMY_UNIT_LIMIT &&
            (old >> 6) + 1 != OV6->unitArmy) {
          return -1;
        }
      } else {
        old = -1;
      }
      if (CHART[Ov000_gUnitStats[OV6->toolUnit & 63].moveType]
               [MAP_TILE_TERRAIN(MAP_TILE_AT(x, y))] != -1) {
        if (old > 0) {
          Ov006_RemoveUnitAt(0, x, y);
        }
        if (Player_CountUnits(OV6->unitArmy) < ARMY_UNIT_LIMIT) {
          if (Unit_Spawn(x, y, OV6->toolUnit & 63)) {
            result = 1;
          }
        }
      } else {
        result = -1;
      }
    }
  }
  if (result > 0) {
    func_ov006_0235f690();
    OV6->recountDelay = 5;
  }
  return result;
}
