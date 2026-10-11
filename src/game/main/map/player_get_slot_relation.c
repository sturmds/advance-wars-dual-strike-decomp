#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/vision.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 gSlotControllers[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* How slot other relates to player: SLOT_RELATION_NONE when other is not
 * in the battle (gSlotControllers is 0), else by team. */
int Player_GetSlotRelation(u16 player, u16 other)
{
    if (gSlotControllers[other] == 0) {
        return SLOT_RELATION_NONE;
    }
    if (STATE->players[player].team != STATE->players[other].team) {
        return SLOT_RELATION_ENEMY;
    }
    return SLOT_RELATION_ALLY;
}
