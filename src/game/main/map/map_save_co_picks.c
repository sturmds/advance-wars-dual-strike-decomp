#include "types.h"
#include "game/Unk_02080440.h"

#include "game/player.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

u8 (*GetCoPicksForMode(int mode))[2];

/* Copies the COs of the players that picked theirs (human, or every player in
 * game modes 2 and 3) into the picks kept between maps. */
void Map_SaveCoPicks(void) {
  u8(*cos)[2];
  int player;
  int slot;

  cos = GetCoPicksForMode(STATE->gameMode);
  if (gLinkPlay != 0) {
    return;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (STATE->gameMode == 3 || STATE->gameMode == 2 ||
        STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
      for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
        cos[player][slot] = STATE->playerCos[player][slot] & 0x7f;
      }
    }
  }
}
