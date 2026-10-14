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
extern u8 gSlotControllers[]; /* per slot: PLAYER_CONTROLLER_* */

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

/* Copies each player's controller (slots 1-4) into gSlotControllers. In a
 * two-map battle where the other map's unk_1c4->unk_30 is 0, a player who is
 * human on the other front is recorded as human too. */
void Players_RecordControllers(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  u32 otherUnk30;
  int i;
  int j;

  SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    gSlotControllers[i] = GAME->players[i].controller;
  }
  SWITCH_BACK(savedGfx, savedGame);
  if (GAME->secondMapId == 0) {
    return;
  }
  SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
  otherUnk30 = GAME->unk_1c4->unk_30;
  SWITCH_BACK(savedGfx, savedGame);
  if (otherUnk30 != 0) {
    return;
  }
  SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
  for (j = 1; j < ARMY_SLOT_COUNT; j++) {
    if (GAME->players[j].controller == PLAYER_CONTROLLER_HUMAN) {
      gSlotControllers[j] = PLAYER_CONTROLLER_HUMAN;
    }
  }
  SWITCH_BACK(savedGfx, savedGame);
}
