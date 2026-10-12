#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

MapObject* Map_FindObjectCovering(int x, int y);
BOOL MapObject_Draw(MapObject* object, int value);
void RangeMap_SetBuffer(void* tileMap);
void RangeMap_Fill(int value);
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

#define GAME (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* B on the map: shows the attack range of what stands on tile (x, y). A map
 * object that MapObject_Draw handles gets its own range. On a unit, a direct
 * attacker gets its move range spread by RangeMap_MarkDirectAttackRange, and an indirect one
 * its fire range around its tile; a unit with neither plays sound 0x10008
 * and returns FALSE. An empty tile goes to Game_State15_EnterState8. */
BOOL MapCursor_ShowRange(int x, int y) {
  MapObject* object;
  u16 direct;
  u16 indirect;

  object = Map_FindObjectCovering(x, y);
  if (object != NULL) {
    RangeMap_SetBuffer(GAME->mapTiles->rangeMapTiles);
    RangeMap_Fill(-1);
    if (MapObject_Draw(object, 0)) {
      if (HOOKS->unk_2c != NULL) {
        HOOKS->unk_2c();
      }
      goto done;
    }
  }
  GAME->actingUnitIndex =
      GAME->mapTiles->unitAt[x + GAME->mapTiles->rowStart[y]];
  GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
  if (GAME->mapTiles->unitAt[x + GAME->mapTiles->rowStart[y]] == 0) {
    Game_State15_EnterState8();
    return TRUE;
  }
  direct = Unit_CanAttackDirect(GAME->actingUnit);
  indirect = Unit_CanAttackIndirect(GAME->actingUnit);
  if (direct == 0 && indirect == 0) {
    Sound_PlaySe(0x10008);
    return FALSE;
  }
  RangeMap_SetBuffer(GAME->mapTiles->rangeMapTiles);
  UnitShowTask_Create(GAME->actingUnit);
  if (HOOKS->unk_2c != NULL) {
    HOOKS->unk_2c();
  }
  func_020c8888();
  if (direct != 0) {
    GAME->unk_1e = (GAME->actingUnitIndex >> 6) + 1;
    RangeMap_FillUnitMoves(GAME->actingUnit);
    GAME->unk_1e = GAME->currentPlayer;
    GAME->rangeMap[y][x] = 0;
    RangeMap_MarkDirectAttackRange(64);
    GAME->rangeMap[y][x] = -1;
  }
  if (indirect != 0) {
    if (direct == 0) {
      RangeMap_Fill(-1);
    }
    RangeMap_MarkAttackRange(GAME->actingUnit->x, GAME->actingUnit->y,
                             GAME->actingUnit);
    if (direct != 0) {
      GAME->rangeMap[y][x - 1] = 0;
      GAME->rangeMap[y][x + 1] = 0;
      GAME->rangeMap[y - 1][x] = 0;
      GAME->rangeMap[y + 1][x] = 0;
    }
  }
done:
  func_020f78b8(1);
  GAME->unk_1c = 6;
  Sound_PlaySe(0x10007);
  return TRUE;
}
