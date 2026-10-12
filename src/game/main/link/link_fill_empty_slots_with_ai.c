#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

u8 Map_GetPlayerCount(u32 mapId);
u8 CoPickList_PickRandom(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the link battle setup script (data_021673dc): every slot of the
 * map with no controller becomes an AI player with a random CO from the pick
 * list, redrawn until no other filled slot has the same front CO. */
void Link_FillEmptySlotsWithAi(void) {
  u8 slot;
  u8 count;
  u8 other;
  u8 co;

  count = Map_GetPlayerCount(STATE->unk_1c4->mapId);
  for (slot = 1; slot <= count; slot++) {
    if (STATE->slotControllers[slot] != PLAYER_CONTROLLER_NONE) {
      continue;
    }
    STATE->slotControllers[slot] = PLAYER_CONTROLLER_AI;
    do {
      co = CoPickList_PickRandom();
      for (other = 1; other < count + 1; other++) {
        if (slot != other &&
            STATE->slotControllers[other] != PLAYER_CONTROLLER_NONE &&
            co == (STATE->playerCos[other][0] & PLAYER_CO_ID_MASK)) {
          break;
        }
      }
    } while (other != count + 1);
    STATE->playerCos[slot][0] = co & PLAYER_CO_ID_MASK;
  }
}
