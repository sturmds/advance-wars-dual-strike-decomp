#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

BOOL TimeLimit_GetRemaining(int* value);
BOOL Game_GetDayLimit(void);
BOOL NumberSpriteTask_IsRunning(void);
void NumberSpriteTask_Start(int style, int x, int y, int firstTile, int palette,
                            Task* parent);

#define GAME (data_027c027c->unk_48[data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* Starts the number task on the second screen unless one runs already or the
 * first map's unk_1c4->unk_30 is set. It shows the time left when there is a
 * time limit (style 0), else the days left under a day limit (style 1), else
 * the day (style 2). */
void NumberSpriteTask_StartCounter(int x, int y, int firstTile, int palette,
                                   Task* parent) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  u32 unk30;
  int seconds;
  int style;

  SWITCH_TO(data_027c0288.unk_00, savedGfx, savedGame);
  unk30 = GAME->unk_1c4->unk_30;
  SWITCH_BACK(savedGfx, savedGame);
  if (unk30 != 0) {
    return;
  }

  if (TimeLimit_GetRemaining(&seconds)) {
    style = 0;
  } else if (Game_GetDayLimit()) {
    style = 1;
  } else {
    style = 2;
  }

  if (NumberSpriteTask_IsRunning()) {
    return;
  }

  SWITCH_TO(data_027c0288.unk_04, savedGfx, savedGame);
  NumberSpriteTask_Start(style, x, y, firstTile, palette, parent);
  SWITCH_BACK(savedGfx, savedGame);
}
