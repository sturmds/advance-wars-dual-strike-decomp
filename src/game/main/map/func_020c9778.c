#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/screen.h"

void func_0200a5e0(int screen);
void Screens_SelectConfigs(void);
void func_020c4018(void);
void CurrentPlayer_LoadPalettes(void);
void func_020c0734(void);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Makes screen 0 current, picks the screen configs, then on screen 0's map
 * state runs the unk_58 hook with the map's weather and tileset,
 * func_020c4018, CurrentPlayer_LoadPalettes and func_020c0734. When SceneMode_IsCurrent(4) is
 * true it does the same on screen 1, without func_020c0734. No direct
 * callers. */
void func_020c9778(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  func_0200a5e0(0);
  Screens_SelectConfigs();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (HOOKS->unk_58 != NULL) {
    HOOKS->unk_58(MAP_STATE->unk_1c4->weather, MAP_STATE->unk_1c4->tileset);
  }
  func_020c4018();
  CurrentPlayer_LoadPalettes();
  func_020c0734();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (SceneMode_IsCurrent(4) == 0) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (HOOKS->unk_58 != NULL) {
    HOOKS->unk_58(MAP_STATE->unk_1c4->weather, MAP_STATE->unk_1c4->tileset);
  }
  func_020c4018();
  CurrentPlayer_LoadPalettes();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
