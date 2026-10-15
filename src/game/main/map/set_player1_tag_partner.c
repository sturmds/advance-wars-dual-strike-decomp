#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

u8 PickFreeCo(int co);

/* Mode 1 only. Gives player 1 a back (tag) CO, Jake if he is free, when
 * the map header's variantMapIds[0] is set or unk_59 is 2 or 3; takes it away when
 * unk_59 is 0 or 1. A back CO already set is kept. */
void SetPlayer1TagPartner(void) {
  /* The writes after PickFreeCo reload the state through the globals;
     * the other accesses use this copy. */
  Unk_02080440_48* state = MAP_STATE;

  if (state->gameMode != GAME_MODE_WAR_ROOM) {
    return;
  }
  if (Ov000_gMapHeaders[state->unk_1c4->mapId].variantMapIds[0] != 0) {
    if ((state->playerCos[1][1] & PLAYER_CO_ID_MASK) == CO_NONE) {
      MAP_STATE->playerCos[1][1] = PickFreeCo(CO_JAKE) & PLAYER_CO_ID_MASK;
    }
    return;
  }
  switch (state->unk_59) {
    case 2:
    case 3:
      if ((state->playerCos[1][1] & PLAYER_CO_ID_MASK) == CO_NONE) {
        MAP_STATE->playerCos[1][1] = PickFreeCo(CO_JAKE) & PLAYER_CO_ID_MASK;
      }
      break;
    case 0:
    case 1:
      if ((state->playerCos[1][1] & PLAYER_CO_ID_MASK) != CO_NONE) {
        state->playerCos[1][1] = CO_NONE;
      }
      break;
  }
}
