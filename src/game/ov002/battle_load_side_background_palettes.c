#include "types.h"
#include "game/battle_scene.h"
#include "game/terrain.h"

void File_ReadTo(void* dest, const char* path);
u8 Player_GetArmyStyle(int playerIndex);
BOOL Battle_IsSideSea(int side); /* TRUE: the side's unit is a naval unit */
void GX_BeginLoadTexPltt(void);  /* guess: maps VRAM bank F to the CPU (LCDC) */
void GX_EndLoadTexPltt(void);    /* guess: undoes GX_BeginLoadTexPltt */

static char data_ov002_02360384[4] = {
#include "data/ov002/data_ov002_02360384.inc.c"
};
static char data_ov002_02360380[4] = {
#include "data/ov002/data_ov002_02360380.inc.c"
};
static char data_ov002_0236037c[4] = {
#include "data/ov002/data_ov002_0236037c.inc.c"
};
static char data_ov002_02360378[4] = {
#include "data/ov002/data_ov002_02360378.inc.c"
};
static char data_ov002_02360374[4] = {
#include "data/ov002/data_ov002_02360374.inc.c"
};
static char data_ov002_02360370[4] = {
#include "data/ov002/data_ov002_02360370.inc.c"
};
static char data_ov002_0236036c[4] = {
#include "data/ov002/data_ov002_0236036c.inc.c"
};
static char data_ov002_02360368[4] = {
#include "data/ov002/data_ov002_02360368.inc.c"
};
static char data_ov002_02360364[4] = {
#include "data/ov002/data_ov002_02360364.inc.c"
};
static char data_ov002_02360360[4] = {
#include "data/ov002/data_ov002_02360360.inc.c"
};
static char data_ov002_0236035c[4] = {
#include "data/ov002/data_ov002_0236035c.inc.c"
};
static char data_ov002_02360358[4] = {
#include "data/ov002/data_ov002_02360358.inc.c"
};
static char data_ov002_02360354[4] = {
#include "data/ov002/data_ov002_02360354.inc.c"
};
static char data_ov002_02360350[4] = {
#include "data/ov002/data_ov002_02360350.inc.c"
};
static char data_ov002_0236034c[4] = {
#include "data/ov002/data_ov002_0236034c.inc.c"
};
static char data_ov002_02360348[4] = {
#include "data/ov002/data_ov002_02360348.inc.c"
};
static char data_ov002_02360344[4] = {
#include "data/ov002/data_ov002_02360344.inc.c"
};
static char data_ov002_02360340[4] = {
#include "data/ov002/data_ov002_02360340.inc.c"
};
static char data_ov002_0236033c[4] = {
#include "data/ov002/data_ov002_0236033c.inc.c"
};
static char data_ov002_02360338[4] = {
#include "data/ov002/data_ov002_02360338.inc.c"
};
static char data_ov002_02360334[4] = {
#include "data/ov002/data_ov002_02360334.inc.c"
};
static char data_ov002_02360330[4] = {
#include "data/ov002/data_ov002_02360330.inc.c"
};
static char data_ov002_0236032c[4] = {
#include "data/ov002/data_ov002_0236032c.inc.c"
};
static char data_ov002_02360328[4] = {
#include "data/ov002/data_ov002_02360328.inc.c"
};
static char data_ov002_02360324[4] = {
#include "data/ov002/data_ov002_02360324.inc.c"
};
static char data_ov002_02360320[4] = {
#include "data/ov002/data_ov002_02360320.inc.c"
};
static char data_ov002_0236031c[4] = {
#include "data/ov002/data_ov002_0236031c.inc.c"
};
static char data_ov002_02360318[4] = {
#include "data/ov002/data_ov002_02360318.inc.c"
};
static char data_ov002_02360314[4] = {
#include "data/ov002/data_ov002_02360314.inc.c"
};
static char data_ov002_02360310[4] = {
#include "data/ov002/data_ov002_02360310.inc.c"
};
static char data_ov002_0236030c[4] = {
#include "data/ov002/data_ov002_0236030c.inc.c"
};
static char data_ov002_02360308[4] = {
#include "data/ov002/data_ov002_02360308.inc.c"
};
static char data_ov002_02360304[4] = {
#include "data/ov002/data_ov002_02360304.inc.c"
};
static char data_ov002_02360300[4] = {
#include "data/ov002/data_ov002_02360300.inc.c"
};
static char data_ov002_023602fc[4] = {
#include "data/ov002/data_ov002_023602fc.inc.c"
};
static char data_ov002_023602f8[4] = {
#include "data/ov002/data_ov002_023602f8.inc.c"
};
static char data_ov002_023602f4[4] = {
#include "data/ov002/data_ov002_023602f4.inc.c"
};
static char data_ov002_023602f0[4] = {
#include "data/ov002/data_ov002_023602f0.inc.c"
};
static char data_ov002_023602ec[4] = {
#include "data/ov002/data_ov002_023602ec.inc.c"
};
static char data_ov002_023602e8[4] = {
#include "data/ov002/data_ov002_023602e8.inc.c"
};
static char data_ov002_023602e4[4] = {
#include "data/ov002/data_ov002_023602e4.inc.c"
};
static char data_ov002_023602e0[4] = {
#include "data/ov002/data_ov002_023602e0.inc.c"
};
static char data_ov002_023602dc[4] = {
#include "data/ov002/data_ov002_023602dc.inc.c"
};
static char data_ov002_023602d8[4] = {
#include "data/ov002/data_ov002_023602d8.inc.c"
};
static char data_ov002_023602d4[4] = {
#include "data/ov002/data_ov002_023602d4.inc.c"
};
static char data_ov002_023602d0[4] = {
#include "data/ov002/data_ov002_023602d0.inc.c"
};
static char data_ov002_023602cc[4] = {
#include "data/ov002/data_ov002_023602cc.inc.c"
};
static char data_ov002_023602c8[4] = {
#include "data/ov002/data_ov002_023602c8.inc.c"
};
static char data_ov002_023602c4[4] = {
#include "data/ov002/data_ov002_023602c4.inc.c"
};
static char data_ov002_023602c0[4] = {
#include "data/ov002/data_ov002_023602c0.inc.c"
};
static char data_ov002_023602bc[4] = {
#include "data/ov002/data_ov002_023602bc.inc.c"
};
static char data_ov002_023602b8[4] = {
#include "data/ov002/data_ov002_023602b8.inc.c"
};
static char data_ov002_023602b4[4] = {
#include "data/ov002/data_ov002_023602b4.inc.c"
};
static char data_ov002_023602b0[4] = {
#include "data/ov002/data_ov002_023602b0.inc.c"
};
static char data_ov002_023602ac[4] = {
#include "data/ov002/data_ov002_023602ac.inc.c"
};
static char data_ov002_023602a8[4] = {
#include "data/ov002/data_ov002_023602a8.inc.c"
};
static char data_ov002_023602a4[4] = {
#include "data/ov002/data_ov002_023602a4.inc.c"
};
static char data_ov002_023602a0[4] = {
#include "data/ov002/data_ov002_023602a0.inc.c"
};
static char data_ov002_0236029c[4] = {
#include "data/ov002/data_ov002_0236029c.inc.c"
};
static char data_ov002_02360298[4] = {
#include "data/ov002/data_ov002_02360298.inc.c"
};
static char data_ov002_02360294[4] = {
#include "data/ov002/data_ov002_02360294.inc.c"
};
static char data_ov002_02360290[4] = {
#include "data/ov002/data_ov002_02360290.inc.c"
};
static char data_ov002_0236028c[4] = {
#include "data/ov002/data_ov002_0236028c.inc.c"
};
static char data_ov002_02360288[4] = {
#include "data/ov002/data_ov002_02360288.inc.c"
};
static char data_ov002_02360284[4] = {
#include "data/ov002/data_ov002_02360284.inc.c"
};
static char data_ov002_02360280[4] = {
#include "data/ov002/data_ov002_02360280.inc.c"
};
static char data_ov002_0236027c[4] = {
#include "data/ov002/data_ov002_0236027c.inc.c"
};
static char data_ov002_02360278[4] = {
#include "data/ov002/data_ov002_02360278.inc.c"
};
static char data_ov002_02360274[4] = {
#include "data/ov002/data_ov002_02360274.inc.c"
};
static char data_ov002_02360270[4] = {
#include "data/ov002/data_ov002_02360270.inc.c"
};
static char data_ov002_0236026c[4] = {
#include "data/ov002/data_ov002_0236026c.inc.c"
};
static char data_ov002_02360268[4] = {
#include "data/ov002/data_ov002_02360268.inc.c"
};
static char data_ov002_02360264[4] = {
#include "data/ov002/data_ov002_02360264.inc.c"
};
static char data_ov002_02360260[4] = {
#include "data/ov002/data_ov002_02360260.inc.c"
};
static char data_ov002_0236025c[4] = {
#include "data/ov002/data_ov002_0236025c.inc.c"
};
static char data_ov002_02360258[4] = {
#include "data/ov002/data_ov002_02360258.inc.c"
};
static char data_ov002_02360254[4] = {
#include "data/ov002/data_ov002_02360254.inc.c"
};
static char data_ov002_02360250[4] = {
#include "data/ov002/data_ov002_02360250.inc.c"
};
static char data_ov002_0236024c[4] = {
#include "data/ov002/data_ov002_0236024c.inc.c"
};
static char data_ov002_02360248[4] = {
#include "data/ov002/data_ov002_02360248.inc.c"
};
static char data_ov002_02360244[4] = {
#include "data/ov002/data_ov002_02360244.inc.c"
};
static char data_ov002_02360240[4] = {
#include "data/ov002/data_ov002_02360240.inc.c"
};
static char data_ov002_0236023c[4] = {
#include "data/ov002/data_ov002_0236023c.inc.c"
};
static char data_ov002_02360238[4] = {
#include "data/ov002/data_ov002_02360238.inc.c"
};
static char data_ov002_02360234[4] = {
#include "data/ov002/data_ov002_02360234.inc.c"
};
static char data_ov002_02360230[4] = {
#include "data/ov002/data_ov002_02360230.inc.c"
};
static char data_ov002_0236022c[4] = {
#include "data/ov002/data_ov002_0236022c.inc.c"
};
static char data_ov002_02360228[4] = {
#include "data/ov002/data_ov002_02360228.inc.c"
};
static char data_ov002_02360224[4] = {
#include "data/ov002/data_ov002_02360224.inc.c"
};
static char data_ov002_02360220[4] = {
#include "data/ov002/data_ov002_02360220.inc.c"
};
static char data_ov002_0236021c[4] = {
#include "data/ov002/data_ov002_0236021c.inc.c"
};
static char data_ov002_02360218[4] = {
#include "data/ov002/data_ov002_02360218.inc.c"
};
static char data_ov002_02360214[4] = {
#include "data/ov002/data_ov002_02360214.inc.c"
};
static char data_ov002_02360210[4] = {
#include "data/ov002/data_ov002_02360210.inc.c"
};
static char data_ov002_0236020c[4] = {
#include "data/ov002/data_ov002_0236020c.inc.c"
};
static char data_ov002_02360208[4] = {
#include "data/ov002/data_ov002_02360208.inc.c"
};
static char data_ov002_02360204[4] = {
#include "data/ov002/data_ov002_02360204.inc.c"
};
static char data_ov002_02360200[4] = {
#include "data/ov002/data_ov002_02360200.inc.c"
};
static char data_ov002_023601fc[4] = {
#include "data/ov002/data_ov002_023601fc.inc.c"
};
static char data_ov002_023601f8[4] = {
#include "data/ov002/data_ov002_023601f8.inc.c"
};
static char data_ov002_023601f4[4] = {
#include "data/ov002/data_ov002_023601f4.inc.c"
};
static char data_ov002_023601f0[4] = {
#include "data/ov002/data_ov002_023601f0.inc.c"
};
static char data_ov002_023601ec[4] = {
#include "data/ov002/data_ov002_023601ec.inc.c"
};
static char data_ov002_023601e8[4] = {
#include "data/ov002/data_ov002_023601e8.inc.c"
};
static char data_ov002_023601e4[4] = {
#include "data/ov002/data_ov002_023601e4.inc.c"
};
static char data_ov002_023601e0[4] = {
#include "data/ov002/data_ov002_023601e0.inc.c"
};
static char data_ov002_023601dc[4] = {
#include "data/ov002/data_ov002_023601dc.inc.c"
};
static char data_ov002_023601d8[4] = {
#include "data/ov002/data_ov002_023601d8.inc.c"
};
static char data_ov002_023601d4[4] = {
#include "data/ov002/data_ov002_023601d4.inc.c"
};
static char data_ov002_023601d0[4] = {
#include "data/ov002/data_ov002_023601d0.inc.c"
};
static char data_ov002_023601cc[4] = {
#include "data/ov002/data_ov002_023601cc.inc.c"
};
static char data_ov002_023601c8[4] = {
#include "data/ov002/data_ov002_023601c8.inc.c"
};
static char data_ov002_023601c4[4] = {
#include "data/ov002/data_ov002_023601c4.inc.c"
};
static char data_ov002_023601c0[4] = {
#include "data/ov002/data_ov002_023601c0.inc.c"
};
static char data_ov002_023601bc[4] = {
#include "data/ov002/data_ov002_023601bc.inc.c"
};
static char data_ov002_023601b8[4] = {
#include "data/ov002/data_ov002_023601b8.inc.c"
};
static char data_ov002_023601b4[4] = {
#include "data/ov002/data_ov002_023601b4.inc.c"
};
static char data_ov002_023601b0[4] = {
#include "data/ov002/data_ov002_023601b0.inc.c"
};
static char data_ov002_023601ac[4] = {
#include "data/ov002/data_ov002_023601ac.inc.c"
};
static char data_ov002_023601a8[4] = {
#include "data/ov002/data_ov002_023601a8.inc.c"
};
static char data_ov002_023601a4[4] = {
#include "data/ov002/data_ov002_023601a4.inc.c"
};
static char data_ov002_023601a0[4] = {
#include "data/ov002/data_ov002_023601a0.inc.c"
};
static char data_ov002_0236019c[4] = {
#include "data/ov002/data_ov002_0236019c.inc.c"
};
static char data_ov002_02360198[4] = {
#include "data/ov002/data_ov002_02360198.inc.c"
};
static char data_ov002_02360194[4] = {
#include "data/ov002/data_ov002_02360194.inc.c"
};
static char data_ov002_02360190[4] = {
#include "data/ov002/data_ov002_02360190.inc.c"
};
static char data_ov002_0236018c[4] = {
#include "data/ov002/data_ov002_0236018c.inc.c"
};
static char data_ov002_02360188[4] = {
#include "data/ov002/data_ov002_02360188.inc.c"
};
static char data_ov002_02360184[4] = {
#include "data/ov002/data_ov002_02360184.inc.c"
};
static char data_ov002_02360180[4] = {
#include "data/ov002/data_ov002_02360180.inc.c"
};
static char data_ov002_0236017c[4] = {
#include "data/ov002/data_ov002_0236017c.inc.c"
};
static char data_ov002_02360178[4] = {
#include "data/ov002/data_ov002_02360178.inc.c"
};
static char data_ov002_02360174[4] = {
#include "data/ov002/data_ov002_02360174.inc.c"
};
static char data_ov002_02360170[4] = {
#include "data/ov002/data_ov002_02360170.inc.c"
};
static char data_ov002_0236016c[4] = {
#include "data/ov002/data_ov002_0236016c.inc.c"
};
static char data_ov002_02360388[4] = {
#include "data/ov002/data_ov002_02360388.inc.c"
};

/* VRAM bank F while it is mapped to the CPU. */
#define VRAM_LCDC_F 0x06890000

/* Picks the two palette files by the map's tileset. */
#define PICK_BY_TILESET(first2, second2, first3, second3, first1, second1, \
                        firstDef, secondDef)                               \
  switch (Ov002_gBattleScene->tileset) {                                   \
    case 2:                                                                \
      firstFile = first2;                                                  \
      secondFile = second2;                                                \
      break;                                                               \
    case 3:                                                                \
      firstFile = first3;                                                  \
      secondFile = second3;                                                \
      break;                                                               \
    case 1:                                                                \
      firstFile = first1;                                                  \
      secondFile = second1;                                                \
      break;                                                               \
    default:                                                               \
      firstFile = firstDef;                                                \
      secondFile = secondDef;                                              \
      break;                                                               \
  }

/* Picks the two palette files of an HQ by the army style of its owner. */
#define PICK_BY_ARMY_STYLE(side, firstDef, secondDef, first2, second2, first3, \
                           second3, first4, second4, first5, second5)          \
  switch (Player_GetArmyStyle(Ov002_gBattleScene->sides[side].tileOwner)) {    \
    default:                                                                   \
      firstFile = firstDef;                                                    \
      secondFile = secondDef;                                                  \
      break;                                                                   \
    case 2:                                                                    \
      firstFile = first2;                                                      \
      secondFile = second2;                                                    \
      break;                                                                   \
    case 3:                                                                    \
      firstFile = first3;                                                      \
      secondFile = second3;                                                    \
      break;                                                                   \
    case 4:                                                                    \
      firstFile = first4;                                                      \
      secondFile = second4;                                                    \
      break;                                                                   \
    case 5:                                                                    \
      firstFile = first5;                                                      \
      secondFile = second5;                                                    \
      break;                                                                   \
  }

/* Loads the palettes of the background behind each side of the battle
 * animation (see Battle_LoadSideBackgrounds), picked the same way: by the
 * terrain the unit stands on (naval units always get the sea), the map's
 * tileset and, for an HQ, the army style of its owner. Each side gets two
 * 16-colour palettes in VRAM bank F: side 0 slots 10 and 12, side 1 slots
 * 11 and 13. */
void Battle_LoadSideBackgroundPalettes(void) {
  int i;
  int isFirst;
  const char* firstFile;
  const char* secondFile;

  GX_BeginLoadTexPltt();
  for (i = 0; i < 2; i++) {
    if (Battle_IsSideSea(i) == TRUE) {
      PICK_BY_TILESET(data_ov002_0236016c, data_ov002_02360170,
                      data_ov002_02360174, data_ov002_02360178,
                      data_ov002_0236017c, data_ov002_02360180,
                      data_ov002_02360184, data_ov002_02360188)
    } else {
      switch (Ov002_gBattleScene->sides[i].terrain) {
        case TERRAIN_RIVER:
          PICK_BY_TILESET(data_ov002_0236018c, data_ov002_02360190,
                          data_ov002_02360194, data_ov002_02360198,
                          data_ov002_0236019c, data_ov002_023601a0,
                          data_ov002_023601a4, data_ov002_023601a8)
          break;
        case TERRAIN_MOUNTAIN:
          PICK_BY_TILESET(data_ov002_023601ac, data_ov002_023601b0,
                          data_ov002_023601b4, data_ov002_023601b8,
                          data_ov002_023601bc, data_ov002_023601c0,
                          data_ov002_023601c4, data_ov002_023601c8)
          break;
        case TERRAIN_WOOD:
          PICK_BY_TILESET(data_ov002_023601cc, data_ov002_023601d0,
                          data_ov002_023601d4, data_ov002_023601d8,
                          data_ov002_023601dc, data_ov002_023601e0,
                          data_ov002_023601e4, data_ov002_023601e8)
          break;
        case TERRAIN_ROAD:
          PICK_BY_TILESET(data_ov002_023601ec, data_ov002_023601f0,
                          data_ov002_023601f4, data_ov002_023601f8,
                          data_ov002_023601fc, data_ov002_02360200,
                          data_ov002_02360204, data_ov002_02360208)
          break;
        case TERRAIN_SEA:
        case TERRAIN_REEF:
          PICK_BY_TILESET(data_ov002_0236016c, data_ov002_02360170,
                          data_ov002_02360174, data_ov002_02360178,
                          data_ov002_0236017c, data_ov002_02360180,
                          data_ov002_02360184, data_ov002_02360188)
          break;
        case TERRAIN_BRIDGE:
          PICK_BY_TILESET(data_ov002_0236020c, data_ov002_02360210,
                          data_ov002_02360214, data_ov002_02360218,
                          data_ov002_0236021c, data_ov002_02360220,
                          data_ov002_02360224, data_ov002_02360228)
          break;
        case TERRAIN_SHOAL:
          PICK_BY_TILESET(data_ov002_0236022c, data_ov002_02360230,
                          data_ov002_02360234, data_ov002_02360238,
                          data_ov002_0236023c, data_ov002_02360240,
                          data_ov002_02360244, data_ov002_02360248)
          break;
        case TERRAIN_AIRPORT:
          PICK_BY_TILESET(data_ov002_0236024c, data_ov002_02360250,
                          data_ov002_02360254, data_ov002_02360258,
                          data_ov002_0236025c, data_ov002_02360260,
                          data_ov002_02360264, data_ov002_02360268)
          break;
        case TERRAIN_PORT:
          PICK_BY_TILESET(data_ov002_0236026c, data_ov002_02360270,
                          data_ov002_02360274, data_ov002_02360278,
                          data_ov002_0236027c, data_ov002_02360280,
                          data_ov002_02360284, data_ov002_02360288)
          break;
        case TERRAIN_BASE:
        case TERRAIN_LAB:
          PICK_BY_TILESET(data_ov002_0236028c, data_ov002_02360290,
                          data_ov002_02360294, data_ov002_02360298,
                          data_ov002_0236029c, data_ov002_023602a0,
                          data_ov002_023602a4, data_ov002_023602a8)
          break;
        case TERRAIN_CITY:
        case TERRAIN_SILO:
        case TERRAIN_SILO_USED:
        case TERRAIN_COM_TOWER:
          PICK_BY_TILESET(data_ov002_023602ac, data_ov002_023602b0,
                          data_ov002_023602b4, data_ov002_023602b8,
                          data_ov002_023602bc, data_ov002_023602c0,
                          data_ov002_023602c4, data_ov002_023602c8)
          break;
        case TERRAIN_HQ:
          switch (Ov002_gBattleScene->tileset) {
            case 2:
              PICK_BY_ARMY_STYLE(i, data_ov002_023602cc, data_ov002_023602d0,
                                 data_ov002_023602d4, data_ov002_023602d8,
                                 data_ov002_023602dc, data_ov002_023602e0,
                                 data_ov002_023602e4, data_ov002_023602e8,
                                 data_ov002_023602ec, data_ov002_023602f0)
              break;
            case 3:
              PICK_BY_ARMY_STYLE(i, data_ov002_023602f4, data_ov002_023602f8,
                                 data_ov002_023602fc, data_ov002_02360300,
                                 data_ov002_02360304, data_ov002_02360308,
                                 data_ov002_0236030c, data_ov002_02360310,
                                 data_ov002_02360314, data_ov002_02360318)
              break;
            case 1:
              PICK_BY_ARMY_STYLE(i, data_ov002_0236031c, data_ov002_02360320,
                                 data_ov002_02360324, data_ov002_02360328,
                                 data_ov002_0236032c, data_ov002_02360330,
                                 data_ov002_02360334, data_ov002_02360338,
                                 data_ov002_0236033c, data_ov002_02360340)
              break;
            default:
              PICK_BY_ARMY_STYLE(i, data_ov002_02360344, data_ov002_02360348,
                                 data_ov002_0236034c, data_ov002_02360350,
                                 data_ov002_02360354, data_ov002_02360358,
                                 data_ov002_0236035c, data_ov002_02360360,
                                 data_ov002_02360364, data_ov002_02360368)
              break;
          }
          break;
        case 31: /* large-object tile (doc 890) */
        default:
          PICK_BY_TILESET(data_ov002_0236036c, data_ov002_02360370,
                          data_ov002_02360374, data_ov002_02360378,
                          data_ov002_0236037c, data_ov002_02360380,
                          data_ov002_02360384, data_ov002_02360388)
          break;
      }
    }
    isFirst = i == 0 ? TRUE : FALSE;
    File_ReadTo((void*)(VRAM_LCDC_F + (isFirst ? 10 : 11) * 32), firstFile);
    File_ReadTo((void*)(VRAM_LCDC_F + (isFirst ? 12 : 13) * 32), secondFile);
  }
  GX_EndLoadTexPltt();
}
