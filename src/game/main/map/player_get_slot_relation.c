#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/vision.h"

extern u8 gSlotControllers[];

/* How slot other relates to player: SLOT_RELATION_NONE when other is not
 * in the battle (gSlotControllers is 0), else by team. */
int Player_GetSlotRelation(u16 player, u16 other) {
  if (gSlotControllers[other] == 0) {
    return SLOT_RELATION_NONE;
  }
  if (MAP_STATE->players[player].team != MAP_STATE->players[other].team) {
    return SLOT_RELATION_ENEMY;
  }
  return SLOT_RELATION_ALLY;
}
