#include "types.h"
#include "game/Unk_02080440.h"

#define MAP_TILE_COUNT() \
  (MAP_STATE->unk_1c4->mapWidth * MAP_STATE->unk_1c4->mapHeight)

/* Packs the hp of every tile that units can attack into out, for battle
 * saves. Each byte is either a tile's hp (bits 0-6) with bit 7 set, or a
 * run of 1 to 127 tiles with no hp; 0xff ends the data. TileHp_Unpack
 * reads it back. */
void TileHp_Pack(u8* out) {
  int i;
  u8 zeros;
  u8* hp;

  hp = MAP_STATE->tileHp;
  zeros = 0;

  for (i = 0; i < MAP_TILE_COUNT(); i++) {
    if ((hp[i] & 0x7f) > 0) {
      if (zeros != 0) {
        *out++ = zeros;
        zeros = 0;
      }
      *out++ = (hp[i] & 0x7f) | 0x80;
    } else {
      zeros++;
      if (zeros == 0x7f) {
        *out++ = zeros;
        zeros = 0;
      }
    }
  }
  if (zeros != 0) {
    *out++ = zeros;
  }
  *out = 0xff;
}
