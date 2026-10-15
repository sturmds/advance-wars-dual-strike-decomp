#include "types.h"
#include "game/screen_other_map.h"

BOOL func_020fa6f8(void);

/* Whether the power meters are drawn for a map state: Unk_02080440_48.unk_5c
 * must be set, and for the second state the other screen's state callback
 * (func_020fa6f8) must not answer TRUE. */
BOOL PowerMeter_IsShown(int mapState) {
  BOOL show = TRUE;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;

  if (MAP_STATE->unk_5c == 0) {
    show = FALSE;
  }
  if (mapState == MAP_STATE_SECOND) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    savedGame = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    if (func_020fa6f8() == TRUE) {
      show = FALSE;
    }
    Screen_SetCurrent(savedGame);
    Screen_SetCurrentGfx(savedGfx);
  }
  return show;
}
