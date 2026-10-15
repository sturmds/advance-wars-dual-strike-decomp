#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_bg.h"

/* First of four BG characters (2x2) for a map tile, palette 2. */
#define MASK_SHOWN 0x100
#define MASK_HIDDEN 0x104

/* Draws one row of 17 map tiles into the layer 3 tile map, starting at map
 * tile (tileX, tileY) and at metatile column startCol (0-31, wrapping) of
 * metatile row row (0-31). Each map tile gets one of two 2x2 BG character
 * patterns: MASK_SHOWN where MapTiles.unk_2338 is nonzero, MASK_HIDDEN where
 * it is zero. Tiles off the map are cleared. */
void MapBg_DrawMaskRow(int startCol, int row, int tileX, int tileY) {
  u16 i;
  u16* map;
  int at;
  int col;

  col = startCol & 31;
  map = data_027c0280->layer3Map + (row & 31) * 64;
  for (i = 0; i < MAP_VIEW_COLUMNS + 1; i++) {
    at = data_02169eb8[col];
    if ((tileX + i) >= 0 && (tileX + i) < MAP_STATE->unk_1c4->mapWidth &&
        tileY >= 0 && tileY < MAP_STATE->unk_1c4->mapHeight) {
      if (MAP_STATE->mapTiles->unk_2338[MAP_STATE->mapTiles->rowStart[tileY] +
                                        (tileX + i)] == 0) {
        map[at] = BG_TILE_PALETTE(2) + MASK_HIDDEN + 0;
        *(map + at + 1) = BG_TILE_PALETTE(2) + MASK_HIDDEN + 1;
        *(map + at + 32) = BG_TILE_PALETTE(2) + MASK_HIDDEN + 2;
        *(map + at + 33) = BG_TILE_PALETTE(2) + MASK_HIDDEN + 3;
      } else {
        map[at] = BG_TILE_PALETTE(2) + MASK_SHOWN + 0;
        *(map + at + 1) = BG_TILE_PALETTE(2) + MASK_SHOWN + 1;
        *(map + at + 32) = BG_TILE_PALETTE(2) + MASK_SHOWN + 2;
        *(map + at + 33) = BG_TILE_PALETTE(2) + MASK_SHOWN + 3;
      }
    } else {
      map[at] = 0;
      *(map + at + 1) = 0;
      *(map + at + 32) = 0;
      *(map + at + 33) = 0;
    }
    col = (col + 1) & 31;
  }
}
