#include "types.h"
#include "game/Unk_02080440.h"

int Map_GetLoadedPlayerCount(void);

/* Needed to match: the slot is multiplied by the player size at run time;
 * players[slot] makes the compiler step a pointer instead. */
#define PLAYER(slot)                                              \
  ((Player*)((u8*)data_027c027c->unk_48[data_027c0284]->players + \
             (slot) * sizeof(Player)))

/* Player slot (1-4) of the army colour, or 0 when no player has it. */
int Battle_FindArmySlot(int army) {
  int slot;
  int found = 0;

  slot = found;
  while (slot < Map_GetLoadedPlayerCount()) {
    slot++;
    if (army == PLAYER(slot)->armyColour) {
      found = slot;
      break;
    }
  }
  return found;
}
