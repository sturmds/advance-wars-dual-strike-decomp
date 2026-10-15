#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

int Player_HasUnitsLeft(int player);

#define SCREEN_STATE(i) ((Unk_02080440*)(&data_027c0288.unk_00)[i])

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 16). Passes unless the player is human and bit 16 of
 * unk_1c4->unk_37 is set; then Player_HasUnitsLeft decides. data_027c0288 holds the
 * state of each screen, so the player is read from the current screen's state. */
int func_020ceffc(int player) {
  if (SCREEN_STATE(data_027c0284)
          ->unk_48[data_027c0284]
          ->players[player]
          .controller != PLAYER_CONTROLLER_HUMAN) {
    return 1;
  }
  if (!(MAP_STATE->unk_1c4->unk_37 & 16)) {
    return 1;
  }
  return Player_HasUnitsLeft(player);
}
