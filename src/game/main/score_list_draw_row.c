#include "types.h"
#include "game/Unk_0229c734.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
/* Draws the row's points, or `tally` while it is below them. */
void ScoreList_DrawRowPoints(int x, int y, int row, int tally);
/* Draws a number as digit sprites. */
void ResultPoint_QueueNumber(int x, int y, int value, int arg3);

#define SCORES data_0229c734

/* Queues the sprites for score row `row` with its top left at (x, y): the
 * count for kinds that have one, the points, a trailing label at x + 120, and
 * the row's caption at x. Rows 32 high (kinds 4, 5, 10) draw their numbers 8
 * lower. Kinds 13 and 17 pick a caption by the first and second CO's nation. */
void ScoreList_DrawRow(int x, int y, int row, int tally) {
  switch (SCORES->rows[row].kind) {
    case 1:
    case 2:
    case 3:
    case 21:
      ScoreList_DrawRowPoints(x + 80, y, row, tally);
      Sprite_QueueAt(6, (x + 120) & 0x1ff, y & 0xff, &data_027c0060,
                     SCORES->paletteSlot << 12, SCORES->partTile[3] + 72);
      break;
    case 4:
    case 5:
    case 10:
      Sprite_QueueAt(6, (x + 36) & 0x1ff, (y + 8) & 0xff, &data_027c0040,
                     SCORES->paletteSlot << 12, SCORES->partTile[3] + 24);
      ResultPoint_QueueNumber(x + 36, y + 8, SCORES->rows[row].count, 0);
      ScoreList_DrawRowPoints(x + 80, y + 8, row, tally);
      Sprite_QueueAt(6, (x + 120) & 0x1ff, (y + 8) & 0xff, &data_027c0060,
                     SCORES->paletteSlot << 12, SCORES->partTile[3] + 72);
      break;
    default:
      Sprite_QueueAt(6, (x + 36) & 0x1ff, y & 0xff, &data_027c0040,
                     SCORES->paletteSlot << 12, SCORES->partTile[3] + 24);
      ResultPoint_QueueNumber(x + 36, y, SCORES->rows[row].count, 0);
      ScoreList_DrawRowPoints(x + 80, y, row, tally);
      Sprite_QueueAt(6, (x + 120) & 0x1ff, y & 0xff, &data_027c0060,
                     SCORES->paletteSlot << 12, SCORES->partTile[3] + 72);
      break;
  }

  switch (SCORES->rows[row].kind) {
    case 1:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c01ac,
                     SCORES->paletteSlot << 12, SCORES->partTile[0]);
      break;
    case 2:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c01ac,
                     SCORES->paletteSlot << 12, SCORES->partTile[0] + 20);
      break;
    case 3:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c01ac,
                     SCORES->paletteSlot << 12, SCORES->partTile[0] + 40);
      break;
    case 21:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c01ac,
                     SCORES->paletteSlot << 12, SCORES->partTile[1]);
      break;
    case 4:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0048,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2] + 16);
      break;
    case 5:
    case 10:
      Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0048,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2]);
      break;
    case 6:
      Sprite_QueueAt(6, (x + 8) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2] + 36);
      break;
    case 7:
    case 11:
      Sprite_QueueAt(6, (x + 8) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2] + 32);
      break;
    case 8:
    case 12:
      Sprite_QueueAt(6, (x + 8) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2] + 40);
      break;
    case 9:
      Sprite_QueueAt(6, (x + 8) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 2) << 12, SCORES->partTile[2] + 44);
      break;
    case 13:
      if (SCORES->nations[0] == 1) {
        Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                       (SCORES->paletteSlot + 3) << 12, SCORES->partTile[4]);
      } else {
        Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                       (SCORES->paletteSlot + 3) << 12,
                       SCORES->partTile[4] + 16);
      }
      break;
    case 14:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 3) << 12, SCORES->partTile[4] + 4);
      break;
    case 15:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 3) << 12, SCORES->partTile[4] + 8);
      break;
    case 16:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 3) << 12, SCORES->partTile[4] + 12);
      break;
    case 17:
      if (SCORES->nations[1] == 1) {
        Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                       (SCORES->paletteSlot + 4) << 12, SCORES->partTile[4]);
      } else {
        Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                       (SCORES->paletteSlot + 4) << 12,
                       SCORES->partTile[4] + 16);
      }
      break;
    case 18:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 4) << 12, SCORES->partTile[4] + 4);
      break;
    case 19:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 4) << 12, SCORES->partTile[4] + 8);
      break;
    case 20:
      Sprite_QueueAt(6, (x + 6) & 0x1ff, y & 0xff, &data_027c0040,
                     (SCORES->paletteSlot + 4) << 12, SCORES->partTile[4] + 12);
      break;
  }
}
