#include "types.h"
#include "game/Unk_02080440.h"

int Map_GetLoadedPlayerCount(void);

#define PLAYER(slot)                                              \
  ((Player*)((u8*)data_027c027c->unk_48[data_027c0284]->players + \
             (slot) * sizeof(Player)))

/* TRUE unless the player with this army colour is human-controlled (also TRUE
 * when no player has the colour: slot 0 has no controller). Battle_PickFrame
 * uses it on the survivor's army. Needed to match: same loop as
 * Battle_FindArmySlot, with the slot multiplied by the player size at run time. */
BOOL Battle_IsArmyNotHuman(int army) {
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
  return PLAYER(found)->controller != PLAYER_CONTROLLER_HUMAN;
}
