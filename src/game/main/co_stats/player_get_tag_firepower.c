#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

u8 Co_GetTagFirepower(int co, int partner);

/* Firepower bonus in percent that the front CO gets from its tag partner;
 * 0 when tagPower is clear. */
int Player_GetTagFirepower(int playerIndex, BOOL tagPower) {
  if (!tagPower) {
    return 0;
  }
  return Co_GetTagFirepower(data_027c027c->unk_48[data_027c0284]
                                    ->players[playerIndex]
                                    .front.id &
                                PLAYER_CO_ID_MASK,
                            data_027c027c->unk_48[data_027c0284]
                                    ->players[playerIndex]
                                    .back.id &
                                PLAYER_CO_ID_MASK) -
         100;
}
