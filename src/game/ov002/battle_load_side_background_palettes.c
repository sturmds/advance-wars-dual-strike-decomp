#include "types.h"
#include "game/battle_scene.h"
#include "game/terrain.h"

void File_ReadTo(void* dest, const char* path);
u8 Player_GetArmyStyle(int playerIndex);
BOOL Battle_IsSideSea(int side); /* TRUE: the side's unit is a naval unit */
void GX_BeginLoadTexPltt(void);  /* guess: maps VRAM bank F to the CPU (LCDC) */
void GX_EndLoadTexPltt(void);    /* guess: undoes GX_BeginLoadTexPltt */

extern char data_ov002_0236016c[];
extern char data_ov002_02360170[];
extern char data_ov002_02360174[];
extern char data_ov002_02360178[];
extern char data_ov002_0236017c[];
extern char data_ov002_02360180[];
extern char data_ov002_02360184[];
extern char data_ov002_02360188[];
extern char data_ov002_0236018c[];
extern char data_ov002_02360190[];
extern char data_ov002_02360194[];
extern char data_ov002_02360198[];
extern char data_ov002_0236019c[];
extern char data_ov002_023601a0[];
extern char data_ov002_023601a4[];
extern char data_ov002_023601a8[];
extern char data_ov002_023601ac[];
extern char data_ov002_023601b0[];
extern char data_ov002_023601b4[];
extern char data_ov002_023601b8[];
extern char data_ov002_023601bc[];
extern char data_ov002_023601c0[];
extern char data_ov002_023601c4[];
extern char data_ov002_023601c8[];
extern char data_ov002_023601cc[];
extern char data_ov002_023601d0[];
extern char data_ov002_023601d4[];
extern char data_ov002_023601d8[];
extern char data_ov002_023601dc[];
extern char data_ov002_023601e0[];
extern char data_ov002_023601e4[];
extern char data_ov002_023601e8[];
extern char data_ov002_023601ec[];
extern char data_ov002_023601f0[];
extern char data_ov002_023601f4[];
extern char data_ov002_023601f8[];
extern char data_ov002_023601fc[];
extern char data_ov002_02360200[];
extern char data_ov002_02360204[];
extern char data_ov002_02360208[];
extern char data_ov002_0236020c[];
extern char data_ov002_02360210[];
extern char data_ov002_02360214[];
extern char data_ov002_02360218[];
extern char data_ov002_0236021c[];
extern char data_ov002_02360220[];
extern char data_ov002_02360224[];
extern char data_ov002_02360228[];
extern char data_ov002_0236022c[];
extern char data_ov002_02360230[];
extern char data_ov002_02360234[];
extern char data_ov002_02360238[];
extern char data_ov002_0236023c[];
extern char data_ov002_02360240[];
extern char data_ov002_02360244[];
extern char data_ov002_02360248[];
extern char data_ov002_0236024c[];
extern char data_ov002_02360250[];
extern char data_ov002_02360254[];
extern char data_ov002_02360258[];
extern char data_ov002_0236025c[];
extern char data_ov002_02360260[];
extern char data_ov002_02360264[];
extern char data_ov002_02360268[];
extern char data_ov002_0236026c[];
extern char data_ov002_02360270[];
extern char data_ov002_02360274[];
extern char data_ov002_02360278[];
extern char data_ov002_0236027c[];
extern char data_ov002_02360280[];
extern char data_ov002_02360284[];
extern char data_ov002_02360288[];
extern char data_ov002_0236028c[];
extern char data_ov002_02360290[];
extern char data_ov002_02360294[];
extern char data_ov002_02360298[];
extern char data_ov002_0236029c[];
extern char data_ov002_023602a0[];
extern char data_ov002_023602a4[];
extern char data_ov002_023602a8[];
extern char data_ov002_023602ac[];
extern char data_ov002_023602b0[];
extern char data_ov002_023602b4[];
extern char data_ov002_023602b8[];
extern char data_ov002_023602bc[];
extern char data_ov002_023602c0[];
extern char data_ov002_023602c4[];
extern char data_ov002_023602c8[];
extern char data_ov002_023602cc[];
extern char data_ov002_023602d0[];
extern char data_ov002_023602d4[];
extern char data_ov002_023602d8[];
extern char data_ov002_023602dc[];
extern char data_ov002_023602e0[];
extern char data_ov002_023602e4[];
extern char data_ov002_023602e8[];
extern char data_ov002_023602ec[];
extern char data_ov002_023602f0[];
extern char data_ov002_023602f4[];
extern char data_ov002_023602f8[];
extern char data_ov002_023602fc[];
extern char data_ov002_02360300[];
extern char data_ov002_02360304[];
extern char data_ov002_02360308[];
extern char data_ov002_0236030c[];
extern char data_ov002_02360310[];
extern char data_ov002_02360314[];
extern char data_ov002_02360318[];
extern char data_ov002_0236031c[];
extern char data_ov002_02360320[];
extern char data_ov002_02360324[];
extern char data_ov002_02360328[];
extern char data_ov002_0236032c[];
extern char data_ov002_02360330[];
extern char data_ov002_02360334[];
extern char data_ov002_02360338[];
extern char data_ov002_0236033c[];
extern char data_ov002_02360340[];
extern char data_ov002_02360344[];
extern char data_ov002_02360348[];
extern char data_ov002_0236034c[];
extern char data_ov002_02360350[];
extern char data_ov002_02360354[];
extern char data_ov002_02360358[];
extern char data_ov002_0236035c[];
extern char data_ov002_02360360[];
extern char data_ov002_02360364[];
extern char data_ov002_02360368[];
extern char data_ov002_0236036c[];
extern char data_ov002_02360370[];
extern char data_ov002_02360374[];
extern char data_ov002_02360378[];
extern char data_ov002_0236037c[];
extern char data_ov002_02360380[];
extern char data_ov002_02360384[];
extern char data_ov002_02360388[];

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
