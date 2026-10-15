#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

extern u8 gSlotControllers[]; /* per slot: PLAYER_CONTROLLER_* */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
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
    gSlotControllers[i] = MAP_STATE->players[i].controller;
  }
  SWITCH_BACK(savedGfx, savedGame);
  if (MAP_STATE->secondMapId == 0) {
    return;
  }
  SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
  otherUnk30 = MAP_STATE->unk_1c4->unk_30;
  SWITCH_BACK(savedGfx, savedGame);
  if (otherUnk30 != 0) {
    return;
  }
  SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
  for (j = 1; j < ARMY_SLOT_COUNT; j++) {
    if (MAP_STATE->players[j].controller == PLAYER_CONTROLLER_HUMAN) {
      gSlotControllers[j] = PLAYER_CONTROLLER_HUMAN;
    }
  }
  SWITCH_BACK(savedGfx, savedGame);
}
