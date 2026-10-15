#include "types.h"
#include "game/math.h"

/* Needed to match: the original reads the duration again at the top of each
 * loop pass; a plain read reuses the value from the loop test. */
#define FRAME_DURATION(frame) (*(volatile const u16*)(frame))

/* Finds the OAM cells to draw for animation at time in a halfword sprite
 * file (all links are halfword offsets). Word 1 leads to the animation
 * table; an animation is a list of (duration, cell index) pairs ending in a
 * pair with duration 0, whose second halfword says what happens after the
 * last frame: 0 stays on the last frame, 1 ends the animation (returns
 * NULL), anything else loops. Cell index n is looked up in the table that
 * word 0 leads to. */
const void* SpriteFile_GetCells(const u16* file, int animation, int time) {
  const u16* cells = file + (file[0] >> 1);
  const u16* table = file + (file[1] >> 1);
  const u16* frame;
  const u16* end;
  int index = table[animation] >> 1;
  int total;

  frame = table + index;
  end = frame;
  total = 0;
  if (table[index] != 0) {
    do {
      total += end[0];
      end += 2;
    } while (end[0] != 0);
  }
  switch (end[1]) {
    case 255:
    default:
      time = HwMod32(time, total);
      break;
    case 0:
      if (time > total) {
        time = total;
      }
      break;
    case 1:
      return NULL;
  }
  while (frame[0] != 0) {
    time -= FRAME_DURATION(frame);
    if (time <= 0) {
      break;
    }
    frame += 2;
  }
  return cells + (cells[frame[1]] >> 1);
}
