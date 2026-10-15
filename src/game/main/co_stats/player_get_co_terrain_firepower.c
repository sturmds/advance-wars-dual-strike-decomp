#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Firepower bonus in percent the front CO of a player gives on a terrain,
 * for its current power state. With coEffectsOn clear it uses Andy's
 * day-to-day value for plains, whatever the CO and terrain. */
int Player_GetCoTerrainFirepower(int playerIndex, int terrain) {
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return gCoTable[CO_ANDY].blocks[POWER_NONE].terrainFirepower[TERRAIN_PLAIN];
  }
  return gCoTable
      [data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
       PLAYER_CO_ID_MASK]
          .blocks[data_027c027c->unk_48[data_027c0284]
                      ->players[playerIndex]
                      .front.powerState]
          .terrainFirepower[terrain];
}
