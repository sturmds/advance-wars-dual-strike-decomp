#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/map_object.h"
#include "game/Unk_02080440.h"

extern u16 data_02157ae4[];   /* texture palette per terrain id */
extern u8 data_02157b24[][2]; /* per tall tile graphic: [0] TILE_DRAW_* */

u8 Map_GetTileColour(int tile);
MapObject* Map_FindObjectCovering(int x, int y);
int MapTile3d_GetTextureOffset(
    int graphic); /* byte offset of a tile graphic in the tile texture */
void Map3d_DrawTile(VecFx32* pos, int tile, int palette, int shaded,
                    int reverse, int swapPairs);
void Map3d_DrawTallTile(VecFx32* pos, int tile, int palette, int shaded);
void MapTile3d_DrawQuadA(VecFx32* pos, int tile, int palette,
                         int shaded); /* 64x64 tile */
void MapTile3d_DrawQuadB(VecFx32* pos, int tile, int palette,
                         int shaded); /* 64x64 tile */

/* data_02157b24[graphic][0] */
#define TILE_DRAW_TALL_OWNED \
  0 /* ground, then a 16x32 graphic in the owner's colours */
#define TILE_DRAW_TALL 1 /* ground, then a 16x32 graphic */
#define TILE_DRAW_OBJECT \
  2 /* ground, then the 64x64 graphic of the map object there */

#define GRAPHIC_FLAT_FIRST \
  79 /* graphics from here on are one flat 16x16 tile; 79 is plain ground */
#define GRAPHIC_DESTROYED_OBJECT 39

/* Writes x, y, z to the geometry engine's MTX_TRANS register (0x04000470). */
static inline void G3_Translate(fx32 x, fx32 y, fx32 z) {
  *REG_MTX_TRANS = x;
  *REG_MTX_TRANS = y;
  *REG_MTX_TRANS = z;
}

/* Draws the map tile at (x, y) with the current matrix: graphic picks the
 * tile picture, tile is the MapTiles.tiles byte (terrain and owner) that
 * picks its palette. highlight 1-3 draws it with palette 5, 15 or 14. The
 * last three flags go to Map3d_DrawTile. */
void Map3d_DrawMapTile(VecFx32* pos, int x, int y, int graphic, u8 tile,
                       int highlight, int shaded, int reverse, int swapPairs) {
  int palette = data_02157ae4[MAP_TILE_TERRAIN(tile)];
  int groundPalette = data_02157ae4[1];
  MapObject* object;

  if (graphic < GRAPHIC_FLAT_FIRST) {
    /* needed to match: the empty cases */
    switch (data_02157b24[graphic][0]) {
      case TILE_DRAW_TALL_OWNED:
        palette += Map_GetTileColour(tile);
        break;
      case TILE_DRAW_TALL:
        break;
      case TILE_DRAW_OBJECT:
        break;
    }
  }
  switch (highlight) {
    case 1:
      groundPalette = palette = 5;
      break;
    case 2:
      groundPalette = palette = 15;
      break;
    case 3:
      groundPalette = palette = 14;
      break;
  }
  if (graphic < GRAPHIC_FLAT_FIRST) {
    switch (data_02157b24[graphic][0]) {
      case TILE_DRAW_TALL_OWNED:
      case TILE_DRAW_TALL:
        Map3d_DrawTile(pos, MapTile3d_GetTextureOffset(GRAPHIC_FLAT_FIRST),
                       groundPalette, shaded, reverse, swapPairs);
        G3_Translate(0, 32, 0);
        Map3d_DrawTallTile(pos, MapTile3d_GetTextureOffset(graphic), palette,
                           shaded);
        return;
      case TILE_DRAW_OBJECT:
        Map3d_DrawTile(pos, MapTile3d_GetTextureOffset(GRAPHIC_FLAT_FIRST),
                       groundPalette, shaded, 0, 0);
        object = Map_FindObjectCovering(x, y);
        if (object == NULL) {
          return;
        }
        G3_Translate(0, 0, 8);
        switch (object->kind) {
          case MAP_OBJECT_BLACK_CANNON:
          case MAP_OBJECT_BLACK_OBELISK:
            if (object->hp == 0) {
              graphic = GRAPHIC_DESTROYED_OBJECT;
            }
            MapTile3d_DrawQuadB(pos, MapTile3d_GetTextureOffset(graphic),
                                palette, shaded);
            break;
          case MAP_OBJECT_VOLCANO:
          case MAP_OBJECT_BLOCKED:
            MapTile3d_DrawQuadA(pos, MapTile3d_GetTextureOffset(graphic),
                                palette, 0);
            break;
        }
        break;
    }
  } else {
    Map3d_DrawTile(pos, MapTile3d_GetTextureOffset(graphic), palette, shaded,
                   reverse, swapPairs);
  }
}
