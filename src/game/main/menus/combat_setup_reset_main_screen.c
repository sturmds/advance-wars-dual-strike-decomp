#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void func_02088514(void);
void MenuButtons_RemoveAll(void);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Removes the menu buttons, clears layer 2 of the main screen, turns its
 * blending off and clears players[].ready of all eight link players. */
void CombatSetup_ResetMainScreen(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;

  MenuButtons_RemoveAll();
  func_02088514();

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Gfx_ClearLayerMap(2);
  Gfx_RequestLayerCopy(2);
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.combat.players); i++) {
    gMenuState.combat.players[i].ready = 0;
  }
}
