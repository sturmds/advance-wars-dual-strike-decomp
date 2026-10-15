#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/map_unit.h"
#include "game/range_map.h"

MapObject* Map_FindObjectCovering(int x, int y);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAttackRange(int x, int y, MapUnit* unit);
void UnitShowTask_Create(MapUnit* unit);
void func_020c8888(void);
void func_020f78b8(int a0);
void Game_State15_EnterState8(void);
void RangeMap_MarkDirectAttackRange(int value);
BOOL Unit_CanAttackDirect(MapUnit* unit);
BOOL Unit_CanAttackIndirect(MapUnit* unit);
void Sound_PlaySe(int se);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* B on the map: shows the attack range of what stands on tile (x, y). A map
 * object that MapObject_MarkFireRange handles gets its fire range. On a unit,
 * a direct attacker gets its move range spread by
 * RangeMap_MarkDirectAttackRange, and an indirect one its fire range around
 * its tile; a unit with neither plays sound 0x10008
 * and returns FALSE. An empty tile goes to Game_State15_EnterState8. */
BOOL MapCursor_ShowRange(int x, int y) {
  MapObject* object;
  u16 direct;
  u16 indirect;

  object = Map_FindObjectCovering(x, y);
  if (object != NULL) {
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
    RangeMap_Fill(-1);
    if (MapObject_MarkFireRange(object, 0)) {
      if (HOOKS->unk_2c != NULL) {
        HOOKS->unk_2c();
      }
      goto done;
    }
  }
  MAP_STATE->actingUnitIndex =
      MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
  MAP_STATE->actingUnit = &MAP_STATE->units[MAP_STATE->actingUnitIndex];
  if (MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]] == 0) {
    Game_State15_EnterState8();
    return TRUE;
  }
  direct = Unit_CanAttackDirect(MAP_STATE->actingUnit);
  indirect = Unit_CanAttackIndirect(MAP_STATE->actingUnit);
  if (direct == 0 && indirect == 0) {
    Sound_PlaySe(0x10008);
    return FALSE;
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  UnitShowTask_Create(MAP_STATE->actingUnit);
  if (HOOKS->unk_2c != NULL) {
    HOOKS->unk_2c();
  }
  func_020c8888();
  if (direct != 0) {
    MAP_STATE->unk_1e = (MAP_STATE->actingUnitIndex >> 6) + 1;
    RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
    MAP_STATE->unk_1e = MAP_STATE->currentPlayer;
    MAP_STATE->rangeMap[y][x] = 0;
    RangeMap_MarkDirectAttackRange(64);
    MAP_STATE->rangeMap[y][x] = -1;
  }
  if (indirect != 0) {
    if (direct == 0) {
      RangeMap_Fill(-1);
    }
    RangeMap_MarkAttackRange(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                             MAP_STATE->actingUnit);
    if (direct != 0) {
      MAP_STATE->rangeMap[y][x - 1] = 0;
      MAP_STATE->rangeMap[y][x + 1] = 0;
      MAP_STATE->rangeMap[y - 1][x] = 0;
      MAP_STATE->rangeMap[y + 1][x] = 0;
    }
  }
done:
  func_020f78b8(1);
  MAP_STATE->unk_1c = 6;
  Sound_PlaySe(0x10007);
  return TRUE;
}
