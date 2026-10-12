#include "types.h"
#include "game/screen_other_map.h"

BOOL Map_IsCoSelectPhase(void);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define FIRST_MAP (data_027c0288.unk_00)
#define CURRENT_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* The day number to show. In a two-map battle whose other map has
 * unk_1c4->unk_30 clear, the other map's day; otherwise the current map's.
 * 1 while players pick their COs. */
u16 Game_GetShownDay(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  Unk_027c0280* otherSavedGfx;
  Unk_02080440* otherSavedGame;
  BOOL useCurrent;
  u32 otherUnk30;
  u16 day;

  SWITCH_TO(FIRST_MAP, savedGfx, savedGame);
  useCurrent = TRUE;
  if (GAME->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP, otherSavedGfx, otherSavedGame);
    otherUnk30 = GAME->unk_1c4->unk_30;
    SWITCH_BACK(otherSavedGfx, otherSavedGame);
    if (otherUnk30 == 0) {
      useCurrent = FALSE;
    }
  }
  SWITCH_BACK(savedGfx, savedGame);

  if (!useCurrent) {
    SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
    day = GAME->day;
    SWITCH_BACK(savedGfx, savedGame);
  } else {
    SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
    day = GAME->day;
    SWITCH_BACK(savedGfx, savedGame);
  }

  SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
  if (Map_IsCoSelectPhase()) {
    day = 1;
  }
  SWITCH_BACK(savedGfx, savedGame);
  return day;
}
