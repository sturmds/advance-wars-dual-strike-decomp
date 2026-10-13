#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* COs the player may pick, ends with CO_NONE; size: up to the next symbol; also declared in src/game/main/map/build_unlocked_co_list.c and 2 more */
u8 data_022a7a68[0x20];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns co if it is in data_022a7a68 and no human player (slotControllers)
 * has it, either as a pick or from the map header. Otherwise returns the
 * first CO of data_022a7a68 that is still free, or CO_NONE. */
u8 PickFreeCo(int co) {
  u8 isFree[CO_COUNT];
  int i;
  int player;
  int slot;
  int taken;

  for (i = 0; i < CO_COUNT; i++) {
    isFree[i] = 0;
  }
  for (i = 0; data_022a7a68[i] != CO_NONE; i++) {
    isFree[data_022a7a68[i]] = 1;
  }

  for (player = 1; player < 5; player++) {
    for (slot = 0; slot < 2; slot++) {
      if (STATE->slotControllers[player] == PLAYER_CONTROLLER_HUMAN) {
        taken = STATE->playerCos[player][slot] & PLAYER_CO_ID_MASK;
        if (taken > CO_NONE && taken < CO_COUNT) {
          isFree[taken] = 0;
        }
        taken = Ov000_gMapHeaders[STATE->unk_1c4->mapId].co[player][slot] &
                PLAYER_CO_ID_MASK;
        if (taken > CO_NONE && taken < CO_COUNT) {
          isFree[taken] = 0;
        }
      }
    }
  }

  if (isFree[co]) {
    return co;
  }
  for (i = 0; data_022a7a68[i] != CO_NONE; i++) {
    if (isFree[data_022a7a68[i]]) {
      return data_022a7a68[i];
    }
  }
  return CO_NONE;
}
