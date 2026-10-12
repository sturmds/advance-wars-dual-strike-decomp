#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_bg.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;

/* Picks a mountain look from the tile position (x is tileX + i). */
#define MOUNTAIN_CELL(y)                                                  \
  gMountainCellVariants[((y) * 2 +                                        \
                         (((y) >> 3) + ((tileX + i) >> 2) + tileX + i)) & \
                        15]

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Draws one row of 17 map tiles into the map BG tile maps, starting at map
 * tile (tileX, tileY) and at metatile column startCol (0-31, wrapping) of
 * metatile row row (0-15). Each map tile is 2x2 BG tiles. Layer 3 gets the
 * tiles of row tileY; layer 2 gets the part of each tile of row tileY + 1
 * that reaches up into this row (guess: mountains and buildings drawn taller
 * than one tile). Tiles off the map are cleared. Mountains pick one of the
 * gMountainCellVariants by position. MapTiles.unk_2338 zero: palette 7, and
 * some terrains (cities, bases, airports, ports, labs, Com Towers) swap in a
 * fixed cell from data_02169e78; nonzero: palette 2 and the map's own cell. */
void MapBg_DrawTileRow(int startCol, int row, int tileX, int tileY) {
  u16 i;
  u16* ground;
  u16* overhang;
  int at;
  int tile;
  int col;
  int terrain;
  int cell;
  int nextY;

  ground = data_027c0280->layer3Map + (row & 15) * 64;
  overhang = data_027c0280->layer2Map + (row & 15) * 64;
  col = startCol & 31;
  nextY = tileY + 1;
  for (i = 0; i < 17; i++) {
    at = data_02169eb8[col];
    if ((tileX + i) >= 0 && (tileX + i) < STATE->unk_1c4->mapWidth &&
        tileY >= 0 && tileY < STATE->unk_1c4->mapHeight) {
      tile = STATE->mapTiles->rowStart[tileY] + (tileX + i);
      terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[tile]);
      if (terrain == TERRAIN_MOUNTAIN) {
        cell = MOUNTAIN_CELL(tileY);
      } else {
        cell = STATE->mapTiles->mapCells[tile];
        if (STATE->mapTiles->unk_2338[tile] == 0 &&
            data_02169e78[terrain] != 0) {
          cell = data_02169e78[terrain];
        }
      }
      if (STATE->mapTiles->unk_2338[tile] == 0) {
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
    if ((tileX + i) >= 0 && (tileX + i) < STATE->unk_1c4->mapWidth &&
        nextY >= 0 && nextY < STATE->unk_1c4->mapHeight) {
      tile = STATE->mapTiles->rowStart[nextY] + (tileX + i);
      terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[tile]);
      if (tileY < STATE->unk_1c4->mapHeight - 1) {
        if (terrain == TERRAIN_MOUNTAIN) {
          cell = MOUNTAIN_CELL(nextY);
        } else {
          cell = STATE->mapTiles->mapCells[tile];
          if (STATE->mapTiles->unk_2338[tile] == 0 &&
              data_02169e78[terrain] != 0) {
            cell = data_02169e78[terrain];
          }
        }
      } else {
        cell = 0;
      }
      if (STATE->mapTiles->unk_2338[tile] == 0) {
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
    col = (col + 1) & 31;
  }
}
