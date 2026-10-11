#include "types.h"
#include "game/vision.h"

extern u8 data_0229eaf0[];

int Player_GetSlotRelation(u16 player, u16 other);

/* TRUE when an ally of player in the battle has data_0229eaf0 kind 1. */
BOOL Player_HasKindOneAlly(u16 player)
{
    int slot;
    u16 i;

    for (i = 1; i < 4; i++) {
        slot = player + i;
        if (Player_GetSlotRelation(player, slot >= 5 ? slot - 4 : slot) == SLOT_RELATION_ALLY) {
            if (slot >= 5) {
                slot -= 4;
            }
            if (data_0229eaf0[slot] == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
