#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE while the player's front CO has a CO Power or Super CO Power on. */
BOOL Player_IsPowerActive(int player) {
  if (data_027c027c->unk_48[data_027c0284]->players[player].front.powerState !=
      POWER_NONE) {
    return TRUE;
  }
  return FALSE;
}
