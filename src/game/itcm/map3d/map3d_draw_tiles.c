#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_02080440.h"
#include "game/camera_pan.h"
#include "game/camera_preset.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;
extern Camera data_022a7d5c;
extern int gCameraPresetIndex;
extern u16 data_02157bc4
    []; /* per map cell code: tile model, with flags 0x4000 and 0x8000 */
extern u16 data_02157f84[]; /* the same for map 248 */
extern int data_022a7c58;   /* tile brightness when no range is shown, 0-31 */
extern int
    data_022a7c60; /* 0: no range shown; else tiles out of range are this much darker (up to 8) */

void Map3d_DrawScrollingQuad(VecFx32* origin, Camera* camera);
BOOL TouchMarker_Exists(void);
int MainLoop_GetFrameCount(
    void); /* needed to match: u32 in its own file, shifted as signed here */
BOOL Map_IsObjectTargetInRange(int x, int y);
void Gx_SetGreyEmission(
    int brightness); /* sets the material emission to grey level brightness */
void Map3d_DrawMapTile(VecFx32* pos, u16 x, u16 y, int model, int tile, int fog,
                       int highlight, int flag4000, int flag8000);
void Map3d_DrawTileUnit(VecFx32* pos, u16 x, u16 y, int highlight,
                        int stateIs8);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* needed to match: the original computes all three values before the first
 * register write. */
static inline void G3_Translate(fx32 x, fx32 y, fx32 z) {
  *REG_MTX_TRANS = x;
  *REG_MTX_TRANS = y;
  *REG_MTX_TRANS = z;
}

/* Draws the 3D map: a 22 by 18 tile window around the camera, each tile
 * with its unit, at origin plus 16 per tile. Tiles outside the map use a
 * border model (border 1: model 79 on plain, else model 120 on sea). With a
 * range shown, tiles in it are full brightness and the rest darker; a
 * touch marker lights the cursor tile too. */
void Map3d_DrawTiles(VecFx32* origin, int border) {
  int index;
  int fog;
  int stateIs8;
  int tile;
  int brightness;
  int startX;
  int highlight;
  int x;
  int y;
  int endX;
  int endY;
  int flag4000;
  int flag8000;
  int range;
  u16* models;
  int touch;
  VecFx32 pos;
  int startY;
  int model;

  *REG_MTX_PUSH = 0;
  startX = ((data_022a7d5c.x - gCameraPresets[gCameraPresetIndex].x) >> 9) - 2;
  startY = ((gCameraPresets[gCameraPresetIndex].y - data_022a7d5c.y) >> 9) - 4;
  endX = startX + 22;
  endY = startY + 18;
  Map3d_DrawScrollingQuad(origin, &data_022a7d5c);
  if (TouchMarker_Exists()) {
    touch = 1;
  } else {
    touch = 0;
  }
  /* passed on to the unit draw; the meaning of state 8 is unknown */
  if (STATE->unk_1c == 8) {
    stateIs8 = 1;
  } else {
    stateIs8 = 0;
  }
  if (STATE == &data_02183b08 && STATE->unk_1c4->mapId == 248) {
    models = data_02157f84;
  } else {
    models = data_02157bc4;
  }
  for (y = startY; y < endY; y++) {
    for (x = startX; x < endX; x++) {
      flag4000 = 0;
      flag8000 = 0;
      highlight = 0;
      brightness = data_022a7c58;
      if (x < 0 || x >= STATE->unk_1c4->mapWidth || y < 0 ||
          y >= STATE->unk_1c4->mapHeight) {
        switch (border) {
          case 0:
          default:
            model = 120;
            tile = TERRAIN_SEA;
            fog = 2;
            break;
          case 1:
            model = 79;
            tile = TERRAIN_PLAIN;
            fog = 3;
            break;
        }
      } else {
        index = x + STATE->mapTiles->rowStart[y];
        model = models[STATE->mapTiles->mapCells[index]];
        /* model 2 animates through 2-5 every 16 frames; fog shows model 1 */
        if (model == 2) {
          if (STATE->mapTiles->unk_2338[index] == 0) {
            model = 1;
          } else {
            model += (MainLoop_GetFrameCount() >> 4) & 3;
          }
        }
        if (model & 0x8000) {
          flag8000 = 1;
        }
        if (model & 0x4000) {
          flag4000 = 1;
        }
        index = x + STATE->mapTiles->rowStart[y];
        tile = STATE->mapTiles->tiles[index];
        if (STATE->mapTiles->unk_2338[index] != 0) {
          fog = 0;
        } else {
          fog = 1;
        }
        if (data_022a7c60 != 0) {
          int terrain = MAP_TILE_TERRAIN(tile);

          /* tiles of a large map object: lit when the object's target tile is in range */
          if (terrain == TERRAIN_OBJECT_PART ||
              terrain == TERRAIN_BLACK_CANNON ||
              terrain == TERRAIN_BLACK_OBELISK) {
            if (data_022a7c60 != 0 && Map_IsObjectTargetInRange(x, y)) {
              highlight = 1;
              brightness = 31;
            } else {
              highlight = 0;
              brightness = 31 - data_022a7c60;
            }
          } else {
            range = STATE->mapTiles->rangeMapTiles[index];
            highlight = range >= 0 ? 1 : 0;
            brightness = range >= 0 ? 31 : 31 - data_022a7c60;
          }
        }
        if (touch) {
          if (x == STATE->cursorX && y == STATE->cursorY) {
            highlight = 1;
          }
        }
      }
      *REG_MTX_PUSH = 0;
      pos.x = origin->x + x * 16 + 8;
      pos.y = origin->y + y * 16 + 16;
      pos.z = origin->z;
      /* 32 world units per map pixel; y grows downwards on the map */
      G3_Translate((pos.x << 13) / 256, (-pos.y * 0x6000) / 768, pos.z);
      if (x >= 0 && x < STATE->unk_1c4->mapWidth && y >= 0 &&
          y < STATE->unk_1c4->mapHeight && fog == 0) {
        Map3d_DrawTileUnit(&pos, x, y, highlight, stateIs8);
      }
      Gx_SetGreyEmission(brightness);
      Map3d_DrawMapTile(&pos, x, y, model, tile, fog, highlight, flag4000,
                        flag8000);
      *REG_MTX_POP = 1;
    }
  }
  *REG_MTX_POP = 1;
}
