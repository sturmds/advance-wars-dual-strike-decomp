#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void RowLockMark_DestroyAll(void);
void func_02088514(void);
void MenuButtons_RemoveAll(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Leaves the Combat setup screen (link play): removes its tasks and menu
 * buttons, clears layers 0 and 1 of the main screen and layer 2 of the sub
 * screen (with its scroll), and unlocks every row (locked, set by
 * CombatSetup_LockRow). */
void CombatSetup_ClearScreens(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;

  RowLockMark_DestroyAll();
  func_02088514();
  MenuButtons_RemoveAll();

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Gfx_ClearLayerMap(0);
  Gfx_RequestLayerCopy(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Gfx_ClearLayerMap(2);
  Gfx_RequestLayerCopy(2);
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  for (i = 1; i < COMBAT_ROW_COUNT + 1; i++) {
    gMenuState.combat.rows[i - 1].locked = 0;
  }
}
