#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"
#include "sdk/fs.h"

static char data_ov003_0236492c[] = {
#include "data/ov003/data_ov003_0236492c.inc.c"
};

u32 File_UnpackTo(void* dest, const void* path);
void Ov003_ReplaceRowSpecialTiles(int row);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88
#define TILE_CODE(x, y) (MAP->cells[(y) * MAP->width + (x)])
#define TERRAIN_AT(x, y) (data_ov003_02365d10[TILE_CODE(x, y)] & 31)
#define ROW_AT(x, y) ((data_ov003_02365d10[TILE_CODE(x, y)] >> 5) & 7)

/* Unpacks the map file named by Ov003State.preset into data_ov003_02364a88,
 * then goes over every tile by its terrain (low 5 bits of data_ov003_02365d10):
 * 3, 16, 21 and 26 set unk_f1a to 100 and the rest set it to 0; 21 and 26 also fill
 * the next of the 4 data_ov003_02369bb8 entries; 8 stores its x and y in unk_38 and unk_39
 * of the tile's row (the cell Ov003_ScrollToSelectedUnit scrolls to). With
 * data_02160ac0->unk_7c set, calls Ov003_ReplaceRowSpecialTiles for each row whose active flag is clear. */
void Ov003_LoadMap(void) {
  int y;
  int row;
  Ov003Marker* marker;
  int x;
  Ov003Marker* cleared;
  int i;

  cleared = data_ov003_02369bb8;
  for (i = 0; i < OV003_MARKER_COUNT; i++, cleared++) {
    cleared->busy = 0;
    cleared->unk_0a = 0;
    cleared->burstCount = 0;
    cleared->cooldown = 0;
  }
  FS_ChangeDir(data_ov003_0236492c);
  File_UnpackTo(MAP, STATE->preset);
  marker = data_ov003_02369bb8;
  for (y = 0; y < MAP->height; y++) {
    for (x = 0; x < MAP->width; x++) {
      switch (TERRAIN_AT(x, y)) {
        case 3:
        case 16:
          MAP->unk_f1a[y * MAP->width + x] = 100;
          break;
        case 21:
        case 26:
          MAP->unk_f1a[y * MAP->width + x] = 100;
          marker->x = x;
          marker->y = y;
          if (TERRAIN_AT(x, y) == 21) {
            marker->kind = 6;
          } else {
            marker->kind = 7;
          }
          marker->unk_08_4 = ROW_AT(x, y);
          marker->busy = 1;
          marker++;
          break;
        case 8:
          MAP->unk_f1a[y * MAP->width + x] = 0;
          STATE->rows[ROW_AT(x, y) - 1].unk_38 = x;
          STATE->rows[ROW_AT(x, y) - 1].unk_39 = y;
          break;
        default:
          MAP->unk_f1a[y * MAP->width + x] = 0;
          break;
      }
    }
  }
  if (data_02160ac0->unk_7c == 0) {
    return;
  }
  for (row = 0; row < STATE->rowCount; row++) {
    if (!STATE->rows[row].active) {
      Ov003_ReplaceRowSpecialTiles(row + 1);
    }
  }
}
