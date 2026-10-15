#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_bg.h"
#include "game/terrain.h"

/* Picks a mountain look from the tile position. */
#define MOUNTAIN_CELL(y) \
  gMountainCellVariants[((((y) >> 3) + (tileX >> 2) + tileX) + (y) * 2) & 15]

/* Draws one column of 13 map tiles into the map BG tile maps, starting at
 * map tile (tileX, tileY) and at metatile row row (0-15, wrapping) of
 * metatile column col (0-31). Same drawing as MapBg_DrawTileRow: layer 3
 * gets each tile, layer 2 the part of the tile below that reaches up. */
void MapBg_DrawTileColumn(int col, int row, int tileX, int tileY) {
  u16 i;
  u16* ground;
  u16* overhang;
  int at;
  int tile;
  int terrain;
  int cell;

  at = (row & 15) * 64;
  ground = data_027c0280->layer3Map + data_02169eb8[col & 31];
  overhang = data_027c0280->layer2Map + data_02169eb8[col & 31];
  for (i = 0; i < MAP_VIEW_ROWS + 1; i++) {
    if (tileX >= 0 && tileX < MAP_STATE->unk_1c4->mapWidth &&
        (tileY + i) >= 0 && (tileY + i) < MAP_STATE->unk_1c4->mapHeight) {
      tile = MAP_STATE->mapTiles->rowStart[tileY + i] + tileX;
      terrain = MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[tile]);
      if (terrain == TERRAIN_MOUNTAIN) {
        cell = MOUNTAIN_CELL(tileY + i);
      } else {
        cell = MAP_STATE->mapTiles->mapCells[tile];
        if (MAP_STATE->mapTiles->unk_2338[tile] == 0 &&
            data_02169e78[terrain] != 0) {
          cell = data_02169e78[terrain];
        }
      }
      if (MAP_STATE->mapTiles->unk_2338[tile] == 0) {
        cell *= 4;
        ground[at] = (gMapCellGroundTiles[cell] & BG_TILE_CHAR_AND_FLIP) +
                     BG_TILE_PALETTE(7);
        *(ground + at + 1) =
            (*(gMapCellGroundTiles + cell + 1) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
        *(ground + at + 32) =
            (*(gMapCellGroundTiles + cell + 2) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
        *(ground + at + 33) =
            (*(gMapCellGroundTiles + cell + 3) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
      } else {
        cell *= 4;
        ground[at] = gMapCellGroundTiles[cell] + BG_TILE_PALETTE(2);
        *(ground + at + 1) =
            *(gMapCellGroundTiles + cell + 1) + BG_TILE_PALETTE(2);
        *(ground + at + 32) =
            *(gMapCellGroundTiles + cell + 2) + BG_TILE_PALETTE(2);
        *(ground + at + 33) =
            *(gMapCellGroundTiles + cell + 3) + BG_TILE_PALETTE(2);
      }
    } else {
      ground[at] = 0;
      *(ground + at + 1) = 0;
      *(ground + at + 32) = 0;
      *(ground + at + 33) = 0;
    }
    if (tileX >= 0 && tileX < MAP_STATE->unk_1c4->mapWidth &&
        (tileY + i + 1) >= 0 &&
        (tileY + i + 1) < MAP_STATE->unk_1c4->mapHeight) {
      tile = MAP_STATE->mapTiles->rowStart[tileY + i + 1] + tileX;
      terrain = MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[tile]);
      if (tileY + i < MAP_STATE->unk_1c4->mapHeight - 1) {
        if (terrain == TERRAIN_MOUNTAIN) {
          cell = MOUNTAIN_CELL(tileY + i + 1);
        } else {
          cell = MAP_STATE->mapTiles->mapCells[tile];
          if (MAP_STATE->mapTiles->unk_2338[tile] == 0 &&
              data_02169e78[terrain] != 0) {
            cell = data_02169e78[terrain];
          }
        }
      } else {
        cell = 0;
      }
      if (MAP_STATE->mapTiles->unk_2338[tile] == 0) {
        cell *= 4;
        overhang[at] = (gMapCellOverhangTiles[cell] & BG_TILE_CHAR_AND_FLIP) +
                       BG_TILE_PALETTE(7);
        *(overhang + at + 1) =
            (*(gMapCellOverhangTiles + cell + 1) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
        *(overhang + at + 32) =
            (*(gMapCellOverhangTiles + cell + 2) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
        *(overhang + at + 33) =
            (*(gMapCellOverhangTiles + cell + 3) & BG_TILE_CHAR_AND_FLIP) +
            BG_TILE_PALETTE(7);
      } else {
        cell *= 4;
        overhang[at] = gMapCellOverhangTiles[cell] + BG_TILE_PALETTE(2);
        *(overhang + at + 1) =
            *(gMapCellOverhangTiles + cell + 1) + BG_TILE_PALETTE(2);
        *(overhang + at + 32) =
            *(gMapCellOverhangTiles + cell + 2) + BG_TILE_PALETTE(2);
        *(overhang + at + 33) =
            *(gMapCellOverhangTiles + cell + 3) + BG_TILE_PALETTE(2);
      }
    } else {
      overhang[at] = 0;
      *(overhang + at + 1) = 0;
      *(overhang + at + 32) = 0;
      *(overhang + at + 33) = 0;
    }
    at = (at + 64) & 0x3ff;
  }
}
