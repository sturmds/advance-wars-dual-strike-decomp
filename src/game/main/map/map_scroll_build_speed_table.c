#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

int abs(int a0);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gMapScrollSpeeds[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Sets up a map scroll: records the current scroll and the distance to the
 * target, then fills gMapScrollSpeeds with the distance to move each frame
 * along the longer axis: 1, 2, 3 ... up to 8, then 8 until the rest is less
 * than the next step, which goes in the last entry. */
void MapScroll_BuildSpeedTable(MapScrollSlot* slot) {
  u16 left;
  u16 speed;

  slot->startX = GAME->unk_1c4->scrollX;
  slot->startY = GAME->unk_1c4->scrollY;
  slot->distX = slot->targetX - GAME->unk_1c4->scrollX;
  slot->distY = slot->targetY - GAME->unk_1c4->scrollY;
  if (abs(slot->distX) > abs(slot->distY)) {
    left = abs(slot->distX);
  } else {
    left = abs(slot->distY);
  }
  slot->step = 0;
  speed = 1;
  /* needed to match: a plain while puts the test at the bottom */
  while (TRUE) {
    if (left - speed < 0) {
      break;
    }
    gMapScrollSpeeds[slot->step] = speed;
    left -= speed;
    if (speed < 8) {
      speed++;
    }
    slot->step++;
  }
  gMapScrollSpeeds[slot->step] = left;
  slot->travelled = 0;
}
