#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

BOOL CanUseCoPowerStrict(int player, int slot);
BOOL CanUseSuperPower(int player, int slot);

#define FRONT(player) (MAP_STATE->players[player].front)

/* Raises the front CO's powerReady when the meter has just filled: 2 once the
 * Super CO Power can be used, 1 once the CO Power can. Returns TRUE when it
 * changed, so the power panel plays the ready sound. It never lowers it. */
BOOL Player_UpdatePowerReady(int player) {
  if (FRONT(player).powerReady < 2 && CanUseSuperPower(player, 0)) {
    FRONT(player).powerReady = 2;
    return TRUE;
  }
  if (FRONT(player).powerReady == 0 && CanUseCoPowerStrict(player, 0)) {
    FRONT(player).powerReady = 1;
    return TRUE;
  }
  return FALSE;
}
