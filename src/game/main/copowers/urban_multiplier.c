#include "types.h"
#include "game/Unk_02080440.h"

void PowerDefaultBonus(void);

extern s8 Ov000_gUrbanTerrain
    []; /* nonzero for urban terrain, indexed by terrain id */

/* Kindle's High Society: +3 firepower for every urban tile the current player
 * owns, on top of the default power bonus. */
void UrbanMultiplier(void) {
  int count;
  int x;
  int y;
  u8 tile;

  count = 0;
  PowerDefaultBonus();
  for (y = 0; y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight;
       y++) {
    for (x = 0; x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth;
         x++) {
      tile =
          data_027c027c->unk_48[data_027c0284]->mapTiles->tiles
              [data_027c027c->unk_48[data_027c0284]->mapTiles->rowStart[y] + x];
      if (Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(tile)] != 0 &&
          MAP_TILE_OWNER(tile) ==
              data_027c027c->unk_48[data_027c0284]->currentPlayer) {
        count++;
      }
    }
  }
  data_027c027c->unk_48[data_027c0284]
      ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
      .firepowerBonus += count * 3;
}
