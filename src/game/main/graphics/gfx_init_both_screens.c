#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/screen.h"

void func_02028d50(void);
void Screen_SetLayerDefaults(void);

extern GfxLayer data_0215943c[];
extern GfxLayer data_0215952c[];

/* Runs the same screen setup (func_02028d50, a config table through
 * Screen_InitLayers, then Screen_SetLayerDefaults) on both screens, each with its own
 * config. The screen states are switched to the screen being set up and
 * back. */
void Gfx_InitBothScreens(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_02028d50();
  Screen_InitLayers(data_0215943c);
  Screen_SetLayerDefaults();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02028d50();
  Screen_InitLayers(data_0215952c);
  Screen_SetLayerDefaults();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
