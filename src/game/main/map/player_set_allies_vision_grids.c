#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/vision.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Player_GetSlotRelation(u16 player, u16 other);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Gives each ally of player the vision grid bits gridMask (Player.visionGrids). */
void Player_SetAlliesVisionGrids(int player, int gridMask) {
  /* needed to match: declaration order and the copies set the registers */
  int ally;
  int i;
  int self;
  int mask;
  mask = gridMask;
  self = player;

  for (i = 1; i < 4; i++) {
    ally = player + i;
    if (ally >= 5) {
      ally -= 4;
    }
    if (Player_GetSlotRelation(self, ally) == SLOT_RELATION_ALLY) {
      STATE->players[ally].visionGrids = mask;
    }
  }
}
