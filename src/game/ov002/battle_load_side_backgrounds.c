#include "types.h"
#include "game/battle_scene.h"
#include "game/terrain.h"

u32 File_UnpackTo(void* dest, const char* path);
u8 Player_GetArmyStyle(int playerIndex);
void GX_BeginLoadTex(void);      /* guess: maps VRAM bank A to the CPU (LCDC) */
void GX_EndLoadTex(void);        /* guess: undoes GX_BeginLoadTex */
BOOL Battle_IsSideSea(int side); /* TRUE: the side's unit is a naval unit */

static char data_ov002_0236038c[4] = {
#include "data/ov002/data_ov002_0236038c.inc.c"
};
char data_ov002_023604a8[4] = {
#include "data/ov002/data_ov002_023604a8.inc.c"
};
char data_ov002_023604a4[4] = {
#include "data/ov002/data_ov002_023604a4.inc.c"
};
char data_ov002_023604a0[4] = {
#include "data/ov002/data_ov002_023604a0.inc.c"
};
char data_ov002_0236049c[4] = {
#include "data/ov002/data_ov002_0236049c.inc.c"
};
static char data_ov002_02360498[4] = {
#include "data/ov002/data_ov002_02360498.inc.c"
};
static char data_ov002_02360494[4] = {
#include "data/ov002/data_ov002_02360494.inc.c"
};
static char data_ov002_02360490[4] = {
#include "data/ov002/data_ov002_02360490.inc.c"
};
static char data_ov002_0236048c[4] = {
#include "data/ov002/data_ov002_0236048c.inc.c"
};
static char data_ov002_02360488[4] = {
#include "data/ov002/data_ov002_02360488.inc.c"
};
static char data_ov002_02360484[4] = {
#include "data/ov002/data_ov002_02360484.inc.c"
};
static char data_ov002_02360480[4] = {
#include "data/ov002/data_ov002_02360480.inc.c"
};
static char data_ov002_0236047c[4] = {
#include "data/ov002/data_ov002_0236047c.inc.c"
};
static char data_ov002_02360478[4] = {
#include "data/ov002/data_ov002_02360478.inc.c"
};
static char data_ov002_02360474[4] = {
#include "data/ov002/data_ov002_02360474.inc.c"
};
static char data_ov002_02360470[4] = {
#include "data/ov002/data_ov002_02360470.inc.c"
};
static char data_ov002_0236046c[4] = {
#include "data/ov002/data_ov002_0236046c.inc.c"
};
static char data_ov002_02360468[4] = {
#include "data/ov002/data_ov002_02360468.inc.c"
};
static char data_ov002_02360464[4] = {
#include "data/ov002/data_ov002_02360464.inc.c"
};
static char data_ov002_02360460[4] = {
#include "data/ov002/data_ov002_02360460.inc.c"
};
static char data_ov002_0236045c[4] = {
#include "data/ov002/data_ov002_0236045c.inc.c"
};
static char data_ov002_02360458[4] = {
#include "data/ov002/data_ov002_02360458.inc.c"
};
static char data_ov002_02360454[4] = {
#include "data/ov002/data_ov002_02360454.inc.c"
};
static char data_ov002_02360450[4] = {
#include "data/ov002/data_ov002_02360450.inc.c"
};
static char data_ov002_0236044c[4] = {
#include "data/ov002/data_ov002_0236044c.inc.c"
};
static char data_ov002_02360448[4] = {
#include "data/ov002/data_ov002_02360448.inc.c"
};
static char data_ov002_02360444[4] = {
#include "data/ov002/data_ov002_02360444.inc.c"
};
static char data_ov002_02360440[4] = {
#include "data/ov002/data_ov002_02360440.inc.c"
};
static char data_ov002_0236043c[4] = {
#include "data/ov002/data_ov002_0236043c.inc.c"
};
static char data_ov002_02360438[4] = {
#include "data/ov002/data_ov002_02360438.inc.c"
};
static char data_ov002_02360434[4] = {
#include "data/ov002/data_ov002_02360434.inc.c"
};
static char data_ov002_02360430[4] = {
#include "data/ov002/data_ov002_02360430.inc.c"
};
static char data_ov002_0236042c[4] = {
#include "data/ov002/data_ov002_0236042c.inc.c"
};
static char data_ov002_02360428[4] = {
#include "data/ov002/data_ov002_02360428.inc.c"
};
static char data_ov002_02360424[4] = {
#include "data/ov002/data_ov002_02360424.inc.c"
};
static char data_ov002_02360420[4] = {
#include "data/ov002/data_ov002_02360420.inc.c"
};
static char data_ov002_0236041c[4] = {
#include "data/ov002/data_ov002_0236041c.inc.c"
};
static char data_ov002_02360418[4] = {
#include "data/ov002/data_ov002_02360418.inc.c"
};
static char data_ov002_02360414[4] = {
#include "data/ov002/data_ov002_02360414.inc.c"
};
static char data_ov002_02360410[4] = {
#include "data/ov002/data_ov002_02360410.inc.c"
};
static char data_ov002_0236040c[4] = {
#include "data/ov002/data_ov002_0236040c.inc.c"
};
static char data_ov002_02360408[4] = {
#include "data/ov002/data_ov002_02360408.inc.c"
};
static char data_ov002_02360404[4] = {
#include "data/ov002/data_ov002_02360404.inc.c"
};
static char data_ov002_02360400[4] = {
#include "data/ov002/data_ov002_02360400.inc.c"
};
static char data_ov002_023603fc[4] = {
#include "data/ov002/data_ov002_023603fc.inc.c"
};
static char data_ov002_023603f8[4] = {
#include "data/ov002/data_ov002_023603f8.inc.c"
};
static char data_ov002_023603f4[4] = {
#include "data/ov002/data_ov002_023603f4.inc.c"
};
static char data_ov002_023603f0[4] = {
#include "data/ov002/data_ov002_023603f0.inc.c"
};
static char data_ov002_023603ec[4] = {
#include "data/ov002/data_ov002_023603ec.inc.c"
};
static char data_ov002_023603e8[4] = {
#include "data/ov002/data_ov002_023603e8.inc.c"
};
static char data_ov002_023603e4[4] = {
#include "data/ov002/data_ov002_023603e4.inc.c"
};
static char data_ov002_023603e0[4] = {
#include "data/ov002/data_ov002_023603e0.inc.c"
};
static char data_ov002_023603dc[4] = {
#include "data/ov002/data_ov002_023603dc.inc.c"
};
static char data_ov002_023603d8[4] = {
#include "data/ov002/data_ov002_023603d8.inc.c"
};
static char data_ov002_023603d4[4] = {
#include "data/ov002/data_ov002_023603d4.inc.c"
};
static char data_ov002_023603d0[4] = {
#include "data/ov002/data_ov002_023603d0.inc.c"
};
static char data_ov002_023603cc[4] = {
#include "data/ov002/data_ov002_023603cc.inc.c"
};
static char data_ov002_023603c8[4] = {
#include "data/ov002/data_ov002_023603c8.inc.c"
};
static char data_ov002_023603c4[4] = {
#include "data/ov002/data_ov002_023603c4.inc.c"
};
static char data_ov002_023603c0[4] = {
#include "data/ov002/data_ov002_023603c0.inc.c"
};
static char data_ov002_023603bc[4] = {
#include "data/ov002/data_ov002_023603bc.inc.c"
};
static char data_ov002_023603b8[4] = {
#include "data/ov002/data_ov002_023603b8.inc.c"
};
static char data_ov002_023603b4[4] = {
#include "data/ov002/data_ov002_023603b4.inc.c"
};
static char data_ov002_023603b0[4] = {
#include "data/ov002/data_ov002_023603b0.inc.c"
};
static char data_ov002_023603ac[4] = {
#include "data/ov002/data_ov002_023603ac.inc.c"
};
static char data_ov002_023603a8[4] = {
#include "data/ov002/data_ov002_023603a8.inc.c"
};
static char data_ov002_023603a4[4] = {
#include "data/ov002/data_ov002_023603a4.inc.c"
};
static char data_ov002_023603a0[4] = {
#include "data/ov002/data_ov002_023603a0.inc.c"
};
static char data_ov002_0236039c[4] = {
#include "data/ov002/data_ov002_0236039c.inc.c"
};
static char data_ov002_02360398[4] = {
#include "data/ov002/data_ov002_02360398.inc.c"
};
static char data_ov002_02360394[4] = {
#include "data/ov002/data_ov002_02360394.inc.c"
};
static char data_ov002_02360390[4] = {
#include "data/ov002/data_ov002_02360390.inc.c"
};
u8 data_ov002_023604ac[12] = {
#include "data/ov002/data_ov002_023604ac.inc.c"
};
u8 data_ov002_023604b8[12] = {
#include "data/ov002/data_ov002_023604b8.inc.c"
};

/* VRAM bank A while it is mapped to the CPU. */
#define VRAM_LCDC_A 0x06800000

/* Picks the background files for tileset 2 or 3, or for any other. */
#define PICK_BY_TILESET(alt1, alt2, def1, def2) \
  switch (Ov002_gBattleScene->tileset) {        \
    case 2:                                     \
    case 3:                                     \
      mapFile = alt1;                           \
      tileFile = alt2;                          \
      break;                                    \
    default:                                    \
      mapFile = def1;                           \
      tileFile = def2;                          \
      break;                                    \
  }

/* Loads the background behind each side of the battle animation from
 * /battle: a 0x1000-byte tile map and 0x2000 bytes of tiles, picked by the
 * terrain the unit stands on (naval units always get the sea), the map's tileset
 * and, for an HQ, the army style of its owner. In VRAM bank A, side 0's
 * tiles go to +0 and its map to +0x4000, side 1's to +0x2000 and +0x5000. */
void Battle_LoadSideBackgrounds(void) {
  int i;
  int isFirst;
  const char* mapFile;
  const char* tileFile;

  GX_BeginLoadTex();
  for (i = 0; i < 2; i++) {
    if (Battle_IsSideSea(i) == TRUE) {
      PICK_BY_TILESET(data_ov002_0236038c, data_ov002_02360390,
                      data_ov002_02360394, data_ov002_02360398)
    } else {
      switch (Ov002_gBattleScene->sides[i].terrain) {
        case TERRAIN_RIVER:
          PICK_BY_TILESET(data_ov002_0236039c, data_ov002_023603a0,
                          data_ov002_023603a4, data_ov002_023603a8)
          break;
        case TERRAIN_MOUNTAIN:
          PICK_BY_TILESET(data_ov002_023603ac, data_ov002_023603b0,
                          data_ov002_023603b4, data_ov002_023603b8)
          break;
        case TERRAIN_WOOD:
          PICK_BY_TILESET(data_ov002_023603bc, data_ov002_023603c0,
                          data_ov002_023603c4, data_ov002_023603c8)
          break;
        case TERRAIN_ROAD:
          PICK_BY_TILESET(data_ov002_023603cc, data_ov002_023603d0,
                          data_ov002_023603d4, data_ov002_023603d8)
          break;
        case TERRAIN_SEA:
        case TERRAIN_REEF:
          PICK_BY_TILESET(data_ov002_0236038c, data_ov002_02360390,
                          data_ov002_02360394, data_ov002_02360398)
          break;
        case TERRAIN_BRIDGE:
          PICK_BY_TILESET(data_ov002_023603dc, data_ov002_023603e0,
                          data_ov002_023603e4, data_ov002_023603e8)
          break;
        case TERRAIN_SHOAL:
          PICK_BY_TILESET(data_ov002_023603ec, data_ov002_023603f0,
                          data_ov002_023603f4, data_ov002_023603f8)
          break;
        case TERRAIN_AIRPORT:
          PICK_BY_TILESET(data_ov002_023603fc, data_ov002_02360400,
                          data_ov002_02360404, data_ov002_02360408)
          break;
        case TERRAIN_PORT:
          PICK_BY_TILESET(data_ov002_0236040c, data_ov002_02360410,
                          data_ov002_02360414, data_ov002_02360418)
          break;
        case TERRAIN_BASE:
        case TERRAIN_LAB:
          PICK_BY_TILESET(data_ov002_0236041c, data_ov002_02360420,
                          data_ov002_02360424, data_ov002_02360428)
          break;
        case TERRAIN_CITY:
        case TERRAIN_SILO:
        case TERRAIN_SILO_USED:
        case TERRAIN_COM_TOWER:
          PICK_BY_TILESET(data_ov002_0236042c, data_ov002_02360430,
                          data_ov002_02360434, data_ov002_02360438)
          break;
        case TERRAIN_HQ:
          switch (Ov002_gBattleScene->tileset) {
            case 2:
            case 3:
              switch (
                  Player_GetArmyStyle(Ov002_gBattleScene->sides[i].tileOwner)) {
                default:
                  mapFile = data_ov002_0236043c;
                  tileFile = data_ov002_02360440;
                  break;
                case 2:
                  mapFile = data_ov002_02360444;
                  tileFile = data_ov002_02360448;
                  break;
                case 3:
                  mapFile = data_ov002_0236044c;
                  tileFile = data_ov002_02360450;
                  break;
                case 4:
                  mapFile = data_ov002_02360454;
                  tileFile = data_ov002_02360458;
                  break;
                case 5:
                  mapFile = data_ov002_0236045c;
                  tileFile = data_ov002_02360460;
                  break;
              }
              break;
            default:
              switch (
                  Player_GetArmyStyle(Ov002_gBattleScene->sides[i].tileOwner)) {
                default:
                  mapFile = data_ov002_02360464;
                  tileFile = data_ov002_02360468;
                  break;
                case 2:
                  mapFile = data_ov002_0236046c;
                  tileFile = data_ov002_02360470;
                  break;
                case 3:
                  mapFile = data_ov002_02360474;
                  tileFile = data_ov002_02360478;
                  break;
                case 4:
                  mapFile = data_ov002_0236047c;
                  tileFile = data_ov002_02360480;
                  break;
                case 5:
                  mapFile = data_ov002_02360484;
                  tileFile = data_ov002_02360488;
                  break;
              }
              break;
          }
          break;
        case 31: /* large-object tile (doc 890) */
        default:
          PICK_BY_TILESET(data_ov002_0236048c, data_ov002_02360490,
                          data_ov002_02360494, data_ov002_02360498)
          break;
      }
    }
    isFirst = i == 0 ? TRUE : FALSE;
    File_UnpackTo((void*)(VRAM_LCDC_A + (isFirst ? 512 : 640) * 32), mapFile);
    File_UnpackTo((void*)(VRAM_LCDC_A + (isFirst ? 0 : 256) * 32), tileFile);
  }
  GX_EndLoadTex();
}
