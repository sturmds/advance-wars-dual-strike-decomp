#include "types.h"
#include "game/second_screen.h"
#include "game/screen_other_map.h"
#include "game/map_unit.h"

int SecondScreenTask_IsRunning(void);
u8 Player_GetArmyStyle(int playerIndex);
void func_ov000_022b0054(void);
void FirstScreenTask_StartOrRefresh(void);
void UnitInfoPanel_Close(void);
void UnitInfoPanel_OpenForUnit(MapUnit* unit);
void NumberSpriteTask_StartCounter(int a, int b, int c, int d, int e);

/* Shows what is under the cursor on the second screen: the unit info panel
 * for a unit, else the tile panel. tile packs the tile's id in bits 0-4 and
 * its owner in bits 5-7; 0 means none. Does nothing unless the second screen
 * task runs. */
void SecondScreen_ShowCursorInfo(int tile, MapUnit* unit) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  if (SecondScreenTask_IsRunning() == 0) {
    return;
  }
  if (tile != 0) {
    data_ov000_02350538 = tile & 31;
    data_ov000_02350534 = tile >> 5;
    data_ov000_02350544 = Player_GetArmyStyle(tile >> 5);
  } else {
    data_ov000_02350538 = 0;
    data_ov000_02350534 = 0;
    data_ov000_02350544 = 0;
  }
  if (unit != NULL && unit->type != 0) {
    func_ov000_022b0054();
    UnitInfoPanel_OpenForUnit(unit);
  } else {
    UnitInfoPanel_Close();
    FirstScreenTask_StartOrRefresh();
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  NumberSpriteTask_StartCounter(181, 0, 708, 15, 3);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
