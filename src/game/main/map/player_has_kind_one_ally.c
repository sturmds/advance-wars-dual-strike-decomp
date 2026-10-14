#include "types.h"
#include "game/vision.h"
#include "game/player.h"

extern u8 gSlotControllers[];

int Player_GetSlotRelation(u16 player, u16 other);

/* TRUE when an ally of player in the battle has gSlotControllers kind 1. */
BOOL Player_HasKindOneAlly(u16 player) {
  int slot;
  u16 i;

  for (i = 1; i < PLAYER_SLOT_LAST; i++) {
    slot = player + i;
    if (Player_GetSlotRelation(
            player, slot >= ARMY_SLOT_COUNT ? slot - PLAYER_SLOT_LAST : slot) ==
        SLOT_RELATION_ALLY) {
      if (slot >= ARMY_SLOT_COUNT) {
        slot -= 4;
      }
      if (gSlotControllers[slot] == 1) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
