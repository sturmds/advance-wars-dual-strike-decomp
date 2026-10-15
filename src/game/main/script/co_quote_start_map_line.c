#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_power_line.h"
#include "game/player.h"

void CoQuote_StartText(u32 script);

/* Looks for a line of the current campaign map that fits the player's front CO
 * and power use count; starts it and returns TRUE when found. */
BOOL CoQuote_StartMapLine(int player) {
  MapPowerLine* line = data_02166f9c[MAP_STATE->unk_1c4->mapId - 224];

  if (line == NULL) {
    return FALSE;
  }
  for (; line->co != -1; line++) {
    if (line->co == (MAP_STATE->players[player].front.id & PLAYER_CO_ID_MASK) &&
        line->powerUses == MAP_STATE->players[player].front.powerUses &&
        (line->player == 0 || line->player == player)) {
      CoQuote_StartText(line->script);
      return TRUE;
    }
  }
  return FALSE;
}
