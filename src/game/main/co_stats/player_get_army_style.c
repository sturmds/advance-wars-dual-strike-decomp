#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Army style of a player slot. */
u8 Player_GetArmyStyle(int playerIndex) {
  return data_027c027c->unk_48[data_027c0284]->players[playerIndex].armyStyle;
}
