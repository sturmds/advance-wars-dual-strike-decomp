#include "types.h"
#include "game/battle_scene.h"
#include "game/terrain.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

u32 File_UnpackTo(void* dest, const char* path);
void File_ReadTo(void* dest, const char* path);
int Battle_IsSideDrawnInAir(int side); /* 1: the side is drawn in the sky */
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);

static char data_ov002_02360164[] = {
#include "data/ov002/data_ov002_02360164.inc.c"
};
static char data_ov002_02360160[] = {
#include "data/ov002/data_ov002_02360160.inc.c"
};
static char data_ov002_0236015c[] = {
#include "data/ov002/data_ov002_0236015c.inc.c"
};
static char data_ov002_02360158[] = {
#include "data/ov002/data_ov002_02360158.inc.c"
};
static char data_ov002_02360154[] = {
#include "data/ov002/data_ov002_02360154.inc.c"
};
static char data_ov002_02360150[] = {
#include "data/ov002/data_ov002_02360150.inc.c"
};
static char data_ov002_0236014c[] = {
#include "data/ov002/data_ov002_0236014c.inc.c"
};
static char data_ov002_02360148[] = {
#include "data/ov002/data_ov002_02360148.inc.c"
};
static char data_ov002_02360144[] = {
#include "data/ov002/data_ov002_02360144.inc.c"
};
static char data_ov002_02360140[] = {
#include "data/ov002/data_ov002_02360140.inc.c"
};
static char data_ov002_0236013c[] = {
#include "data/ov002/data_ov002_0236013c.inc.c"
};
static char data_ov002_02360138[] = {
#include "data/ov002/data_ov002_02360138.inc.c"
};
static char data_ov002_02360134[] = {
#include "data/ov002/data_ov002_02360134.inc.c"
};
static char data_ov002_02360130[] = {
#include "data/ov002/data_ov002_02360130.inc.c"
};
static char data_ov002_0236012c[] = {
#include "data/ov002/data_ov002_0236012c.inc.c"
};
static char data_ov002_02360128[] = {
#include "data/ov002/data_ov002_02360128.inc.c"
};
static char data_ov002_02360124[] = {
#include "data/ov002/data_ov002_02360124.inc.c"
};
static char data_ov002_02360120[] = {
#include "data/ov002/data_ov002_02360120.inc.c"
};
static char data_ov002_0236011c[] = {
#include "data/ov002/data_ov002_0236011c.inc.c"
};
static char data_ov002_02360118[] = {
#include "data/ov002/data_ov002_02360118.inc.c"
};
static char data_ov002_02360114[] = {
#include "data/ov002/data_ov002_02360114.inc.c"
};
static char data_ov002_02360110[] = {
#include "data/ov002/data_ov002_02360110.inc.c"
};
static char data_ov002_0236010c[] = {
#include "data/ov002/data_ov002_0236010c.inc.c"
};
static char data_ov002_02360108[] = {
#include "data/ov002/data_ov002_02360108.inc.c"
};
static char data_ov002_02360104[] = {
#include "data/ov002/data_ov002_02360104.inc.c"
};
static char data_ov002_02360100[] = {
#include "data/ov002/data_ov002_02360100.inc.c"
};
static char data_ov002_023600fc[] = {
#include "data/ov002/data_ov002_023600fc.inc.c"
};
static char data_ov002_023600f8[] = {
#include "data/ov002/data_ov002_023600f8.inc.c"
};
static char data_ov002_023600f4[] = {
#include "data/ov002/data_ov002_023600f4.inc.c"
};
static char data_ov002_023600f0[] = {
#include "data/ov002/data_ov002_023600f0.inc.c"
};
static char data_ov002_023600ec[] = {
#include "data/ov002/data_ov002_023600ec.inc.c"
};
static char data_ov002_02360168[] = {
#include "data/ov002/data_ov002_02360168.inc.c"
};

#define CHAR_BASE(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Side 0 uses BG palette 0, side 1 BG palette 4. */
#define SIDE_PALETTE(side) \
  (data_027c0280->paletteBuffer + ((side) == 0 ? 0 : 4) * 32)

/* A tile map entry's palette number sits in bits 12-15. */
#define MAP_PALETTE(n) ((n) << 12)
#define MAP_ENTRIES (32 * 32)

/* Loads the tiles, map and palette of one side. */
#define LOAD_LAYER(tileFile, mapFile, paletteFile) \
  File_UnpackTo(tiles, tileFile);                  \
  File_UnpackTo(map, mapFile);                     \
  File_ReadTo(SIDE_PALETTE(side), paletteFile)

/* Tilesets 2 and 3 share their tiles and map, as do tileset 1 and the
 * others; each tileset has its own palette. */
#define PICK_BY_TILESET(tiles23, map23, pal2, pal3, tiles1, map1, pal1, \
                        palDefault)                                     \
  switch (Ov002_gBattleScene->tileset) {                                \
    case 2:                                                             \
      LOAD_LAYER(tiles23, map23, pal2);                                 \
      break;                                                            \
    case 3:                                                             \
      LOAD_LAYER(tiles23, map23, pal3);                                 \
      break;                                                            \
    case 1:                                                             \
      LOAD_LAYER(tiles1, map1, pal1);                                   \
      break;                                                            \
    default:                                                            \
      LOAD_LAYER(tiles1, map1, palDefault);                             \
      break;                                                            \
  }

/* Loads the terrain layer of each side of the battle animation into BG2
 * (side 0) and BG3 (side 1): the sky when the side is drawn in the air,
 * otherwise woods, mountains, buildings or plain ground by the terrain the
 * unit stands on. Side 1's map is then moved to BG palette 4. */
void Battle_LoadSideTerrainLayers(void) {
  int side;
  u8* tiles;
  u16* map;
  int i;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    if (side == 0) {
      tiles = CHAR_BASE(data_027c0280->layer2Bg);
      map = data_027c0280->layer2Map;
    } else {
      tiles = CHAR_BASE(data_027c0280->layer3Bg);
      map = data_027c0280->layer3Map;
    }
    if (Battle_IsSideDrawnInAir(side) == 1) {
      File_UnpackTo(tiles, data_ov002_023600ec);
      File_UnpackTo(map, data_ov002_023600f0);
      if (Ov002_gBattleScene->tileset == 1) {
        File_ReadTo(SIDE_PALETTE(side), data_ov002_023600f4);
      } else {
        File_ReadTo(SIDE_PALETTE(side), data_ov002_023600f8);
      }
      continue;
    }
    switch (Ov002_gBattleScene->sides[side].terrain) {
      case TERRAIN_WOOD:
        PICK_BY_TILESET(data_ov002_023600fc, data_ov002_02360100,
                        data_ov002_02360104, data_ov002_02360108,
                        data_ov002_0236010c, data_ov002_02360110,
                        data_ov002_02360114, data_ov002_02360118)
        break;
      case TERRAIN_MOUNTAIN:
        PICK_BY_TILESET(data_ov002_0236011c, data_ov002_02360120,
                        data_ov002_02360124, data_ov002_02360128,
                        data_ov002_0236012c, data_ov002_02360130,
                        data_ov002_02360134, data_ov002_02360138)
        break;
      case TERRAIN_CITY:
      case TERRAIN_HQ:
      case TERRAIN_AIRPORT:
      case TERRAIN_PORT:
      case TERRAIN_BASE:
      case TERRAIN_SILO:
      case TERRAIN_SILO_USED:
      case TERRAIN_LAB:
      case TERRAIN_COM_TOWER:
        PICK_BY_TILESET(data_ov002_0236013c, data_ov002_02360140,
                        data_ov002_02360144, data_ov002_02360148,
                        data_ov002_0236014c, data_ov002_02360150,
                        data_ov002_02360154, data_ov002_02360158)
        break;
      default:
        File_UnpackTo(tiles, data_ov002_0236015c);
        File_UnpackTo(map, data_ov002_02360160);
        if (Ov002_gBattleScene->tileset == 1) {
          File_ReadTo(SIDE_PALETTE(side), data_ov002_02360164);
        } else {
          File_ReadTo(SIDE_PALETTE(side), data_ov002_02360168);
        }
        break;
    }
  }
  for (i = 0; i < MAP_ENTRIES; i++) {
    data_027c0280->layer3Map[i] += MAP_PALETTE(4);
  }
  Gfx_RequestLayerCopy(2);
  Gfx_RequestLayerCopy(3);
  Palette_Flush();
}
