#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/tile_point.h"
#include "sdk/os.h"
#include "game/screen.h"

BOOL Unit_FindOtherMapTile(MapUnit* unit, TilePoint* pos);
void Unit_TransferToOtherMap(MapUnit* unit, int x, int y);
void func_020c8888(void);
void Map_RebuildUnitTiles(void);

/* Moves a unit to the other map of a dual-front battle: finds its tile
 * there (stops the game if there is none), has Unit_TransferToOtherMap put the unit
 * there, then rebuilds the unit tiles on both screens' maps. */
void Unit_MoveToOtherMap(MapUnit* unit) {
  TilePoint pos;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  pos.x = unit->x;
  pos.y = unit->y;
  if (!Unit_FindOtherMapTile(unit, &pos)) {
    OS_Terminate();
  }
  Unit_TransferToOtherMap(unit, pos.x, pos.y);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_020c8888();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (SceneMode_IsCurrent(4)) {
    func_020c8888();
  } else {
    Map_RebuildUnitTiles();
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
