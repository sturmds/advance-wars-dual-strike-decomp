#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define CURRENT_MAP ((&data_027c0288.unk_00)[data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* data_022a7b80.mode values of the Survival modes with a budget in days. */
#define SURVIVAL_MODE_A 2
#define SURVIVAL_MODE_B 5

/* Days the battle may last, 0 for no limit (a guess: the value is read from
 * the map header and shown as days left). Survival modes with a day budget
 * return the budget that is left. */
int Game_GetDayLimit(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  u8 limit;

  if (GAME->gameMode == GAME_MODE_SURVIVAL &&
      (data_022a7b80.mode == SURVIVAL_MODE_A ||
       data_022a7b80.mode == SURVIVAL_MODE_B)) {
    return data_022a7b80.budgetLeft;
  }
  SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
  limit = GAME->dayLimit;
  SWITCH_BACK(savedGfx, savedGame);
  return limit;
}
