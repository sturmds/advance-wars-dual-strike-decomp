#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/move_path.h"
#include "game/play_records.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_0229d8c0;
extern u8 data_022a6b34[];
extern u8 data_022a6b30;
extern PlayRecords gPlayRecords;

BOOL Map_IsFreeDeployCity(int x, int y);
void BuildMenu_Open(int x, int y);
void func_020bc56c(void);
int MapScript_RunList08(MapUnit* unit);
void UnitShowTask_Create(MapUnit* unit);
void func_020c8888(void);
void RangeMap_SetBuffer(void* tileMap);
void RangeMap_FillUnitMoves(MapUnit* unit);
void func_020f78b8(int a0);
void MovePath_Start(void);
void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Acts on the tile (x, y) the player picked on the map. On an empty
 * production tile (Map_IsFreeDeployCity) it calls BuildMenu_Open when allowBuild is
 * set. On a unit that has not waited it starts that unit's move: shows the
 * unit, fills its move range and counts it in gPlayRecords for a human
 * player. Otherwise it opens func_020bc56c when allowMenu is set. Returns
 * FALSE only when nothing was done. */
BOOL MapCursor_SelectTile(int x, int y, int allowBuild, int allowMenu) {
  data_0229d8c0 = 0;
  data_022a6b34[0] = 0;
  data_022a6b34[1] = 0;
  data_022a6b30 = 0;
  GAME->actingUnitIndex =
      GAME->mapTiles->unitAt[GAME->mapTiles->rowStart[y] + x];
  GAME->actingUnit = &GAME->units[GAME->actingUnitIndex];
  if (Map_IsFreeDeployCity(x, y)) {
    if (allowBuild) {
      BuildMenu_Open(x, y);
    }
    return TRUE;
  }
  if (GAME->mapTiles->unitAt[GAME->mapTiles->rowStart[y] + x] == 0 ||
      (GAME->actingUnit->flags & MAP_UNIT_WAITED)) {
    if (!allowMenu) {
      return FALSE;
    }
    func_020bc56c();
    return TRUE;
  }
  MapScript_RunList08(GAME->actingUnit);
  GAME->pathSteps[0] = PATH_STEP_END;
  UnitShowTask_Create(GAME->actingUnit);
  if (data_027c027c->unk_50[data_027c0284]->unk_30 != NULL) {
    data_027c027c->unk_50[data_027c0284]->unk_30();
  }
  func_020c8888();
  /* the move range is filled as seen by the unit's owner */
  GAME->unk_1e = (GAME->actingUnitIndex >> 6) + 1;
  RangeMap_SetBuffer(GAME->mapTiles->rangeMapTiles);
  RangeMap_FillUnitMoves(GAME->actingUnit);
  GAME->unk_1e = GAME->currentPlayer;
  func_020f78b8(0);
  GAME->unk_1c = 1;
  MovePath_Start();
  Sound_PlaySe(0x10007);
  if (GAME->players[GAME->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsSelected < 999999) {
      gPlayRecords.unitsSelected++;
    }
  }
  return TRUE;
}
