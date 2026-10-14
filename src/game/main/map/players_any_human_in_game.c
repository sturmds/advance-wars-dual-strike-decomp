#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])
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

BOOL Player_IsInGame(u8 player);

/* TRUE when a player still in the game is human on this map or, while the
 * other map's unk_1c4->unk_30 is 0, on the other front of a two-map battle.
 * Same test as Players_RecordControllers. */
BOOL Players_AnyHumanInGame(void) {
  BOOL anyHuman;
  Unk_027c0280* savedGfx;
  Unk_027c0280* outerGfx;
  int i;
  Unk_02080440* probeGame;
  Unk_027c0280* probeGfx;
  u32 otherUnk30;
  Unk_02080440* savedGame;
  Unk_02080440* outerGame;

  /* needed to match: the declaration order sets the registers */
  anyHuman = FALSE;
  SWITCH_TO(CURRENT_MAP, outerGfx, outerGame);
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (!Player_IsInGame(i)) {
      continue;
    }
    if (GAME->players[i].controller == PLAYER_CONTROLLER_HUMAN) {
      anyHuman = TRUE;
    }
    if (GAME->secondMapId == 0) {
      continue;
    }
    SWITCH_TO(OTHER_MAP, probeGfx, probeGame);
    otherUnk30 = GAME->unk_1c4->unk_30;
    SWITCH_BACK(probeGfx, probeGame);
    if (otherUnk30 != 0) {
      continue;
    }
    SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
    if (GAME->players[i].controller == PLAYER_CONTROLLER_HUMAN) {
      anyHuman = TRUE;
    }
    SWITCH_BACK(savedGfx, savedGame);
  }
  SWITCH_BACK(outerGfx, outerGame);
  return anyHuman;
}
