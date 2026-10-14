#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_02168220[];
extern u8 data_02168228[];
extern u8 data_02168230[];
extern u8 data_02168240[];
extern u8 data_02168248[];
extern u8 data_02168250[];

void MapObjects_QueueSpritesInRect(int left, int top, int width, int height);
/* The original passes a fifth argument, always 0, that Map_QueueTileSprite
 * does not read. */
void Map_QueueTileSprite(int x, int y, void* data, int palette, int unused);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define SITE (STATE->unk_608c[i])

/* Queues the sprites drawn over the visible map tiles: map objects, then
 * from the site list each minicannon (one of four sprites by the owner bits
 * of its tile byte, probably its facing), terrain 25 and Black Crystal. The
 * view covers 17 by 13 tiles from the scroll position, one less at the
 * right or bottom map edge. */
void MapView_QueueTileSprites(void) {
  int width;
  s16 left;
  s16 top;
  int i;
  u8 tile;
  int terrain;
  int owner;
  int height;

  width = 16;
  height = 12;
  left = STATE->unk_1c4->scrollX / 16;
  top = STATE->unk_1c4->scrollY / 16;

  if (left + 16 == STATE->unk_1c4->mapWidth) {
    width--;
  }
  if (top + 12 == STATE->unk_1c4->mapHeight) {
    height--;
  }
  MapObjects_QueueSpritesInRect(left, top, width, height);
  for (i = 0; i < MAP_SITE_COUNT; i++) {
    tile = SITE.tile;
    if (tile == MAP_SITE_END) {
      return;
    }
    if (left > SITE.x || SITE.x > left + width || top > SITE.y ||
        SITE.y > top + height) {
      continue;
    }
    owner = MAP_TILE_OWNER(tile);
    terrain = MAP_TILE_TERRAIN(tile);
    switch (terrain) {
      case TERRAIN_MINICANNON:
        switch (owner) {
          case 0:
          default:
            Map_QueueTileSprite(SITE.x, SITE.y, data_02168240, 8, 0);
            break;
          case 1:
            Map_QueueTileSprite(SITE.x, SITE.y, data_02168250, 8, 0);
            break;
          case 2:
            Map_QueueTileSprite(SITE.x, SITE.y, data_02168228, 8, 0);
            break;
          case 3:
            Map_QueueTileSprite(SITE.x, SITE.y, data_02168230, 8, 0);
            break;
        }
        break;
      case 25:
        Map_QueueTileSprite(SITE.x, SITE.y, data_02168220, 8, 0);
        break;
      case TERRAIN_BLACK_CRYSTAL:
        Map_QueueTileSprite(SITE.x, SITE.y, data_02168248, 8, 0);
        break;
      case 0xff:
        return; /* cannot happen: terrain is 5 bits */
    }
  }
}
