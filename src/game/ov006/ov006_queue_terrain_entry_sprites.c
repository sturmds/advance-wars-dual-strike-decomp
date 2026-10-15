#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
BOOL func_ov006_023576c4(void);

extern u8 data_ov006_0236a7ec[OV006_TERRAIN_ENTRY_COUNT][2];
extern void* data_ov006_0236a878[OV006_TERRAIN_ENTRY_COUNT];
extern void* data_ov006_0236a8c0[OV006_TERRAIN_ENTRY_COUNT];

#define OV6 data_ov006_0236d640

/* OAM attribute 0 bits */
#define OAM_AFFINE 0x100 /* rotation and scaling on (matrix 0 here) */
#define OAM_SEMI_TRANSPARENT 0x400

/* Queues the sprites of the terrain entries in data_ov006_0236a7a0:
 * per entry one sprite at its position from data_ov006_0236a7ec and one
 * below it (palette 15). Property terrain uses palette 8, the rest 9 to
 * 14. The HQ is left out while the army (terrainPage) is 0, and when prevTerrainPage is
 * 0 and func_ov006_023576c4 returns TRUE. Flag 8 of unk_02 makes both
 * sprites of every entry affine; flag 4 the first sprite of properties. */
void Ov006_QueueTerrainEntrySprites(void) {
  u8* pos;
  s8* terrain;
  int i;

  pos = data_ov006_0236a7ec[0];
  terrain = data_ov006_0236a7a0.ids;

  for (i = 0; i < OV006_TERRAIN_ENTRY_COUNT; i++) {
    int palette = 9;
    BOOL topAffine = FALSE;
    BOOL hide;
    BOOL bottomAffine = FALSE;
    u8 x = *pos++;
    u8 y = *pos++;

    /* needed to match: hide is set after the reads */
    hide = FALSE;
    if (OV6->unk_02 & 8) {
      topAffine = TRUE;
      bottomAffine = TRUE;
    }
    switch (*terrain) {
      case TERRAIN_HQ:
        if (OV6->prevTerrainPage == 0 && func_ov006_023576c4()) {
          hide = TRUE;
        }
        if (OV6->terrainPage == 0) {
          hide = TRUE;
        }
        /* fall through */
      case TERRAIN_CITY:
      case TERRAIN_AIRPORT:
      case TERRAIN_PORT:
      case TERRAIN_BASE:
      case TERRAIN_COM_TOWER:
        palette = 8;
        if (OV6->unk_02 & 4) {
          topAffine = TRUE;
        }
        break;
      case TERRAIN_PLAIN:
      case TERRAIN_WOOD:
        palette = 9;
        break;
      case TERRAIN_MOUNTAIN:
        palette = 10;
        break;
      case TERRAIN_RIVER:
      case TERRAIN_SEA:
      case TERRAIN_SHOAL:
      case TERRAIN_REEF:
        palette = 11;
        break;
      case TERRAIN_ROAD:
      case TERRAIN_BRIDGE:
        palette = 12;
        break;
      case TERRAIN_PIPE:
      case TERRAIN_PIPE_SEAM:
        palette = 13;
        break;
      case TERRAIN_SILO:
        palette = 14;
        break;
    }
    if (hide) {
      terrain++;
      continue;
    }
    Sprite_Queue(
        0, x & 0x1ff,
        ((y - 8) & 0xff) | OAM_SEMI_TRANSPARENT | (topAffine ? OAM_AFFINE : 0),
        data_ov006_0236a878[i], palette << 12, 0);
    Sprite_Queue(0, (x - 8) & 0x1ff,
                 ((y + 26) & 0xff) | OAM_SEMI_TRANSPARENT |
                     (bottomAffine ? OAM_AFFINE : 0),
                 data_ov006_0236a8c0[i], 15 << 12, 0);
    terrain++;
  }
}
