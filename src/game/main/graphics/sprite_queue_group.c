#include "types.h"
#include "game/Unk_02088d04.h"

const void* Oam_GetAttrCells(const u16* attr);

#define OBJ_X_FLIP 0x1000

/* Queues a group of sprites on sprite list list at (x, y). group starts
 * with the sprite count, then three halfwords per sprite: OAM attribute 0
 * with a signed y offset in its low byte, attribute 1 with a signed 9-bit
 * x offset, and the tile, to which tileAdd is added. xBits and yBits are
 * or'd into the attributes; OBJ_X_FLIP in xBits mirrors the x offsets.
 * Sprites whose position is off screen by 64 pixels or more are skipped.
 * The bits are also added to the position; callers pass them with the
 * position bits clear. */
void Sprite_QueueGroup(int list, int x, int y, u16 xBits, u16 yBits,
                       const u16* group, u16 tileAdd, u16 attr) {
  int count;
  int i;
  s8 dy;
  s16 dx;

  count = *group++;
  for (i = 0; i < count; i++, group += 3) {
    dy = group[0];
    if (y + dy <= -64 || y + dy >= 192) {
      continue;
    }
    dx = group[1] & 0x1ff;
    if (dx & 0x100) {
      dx |= 0xff00;
    }
    if (xBits & OBJ_X_FLIP) {
      dx = -dx;
    }
    if (dx + x <= -64 || dx + x >= 256) {
      continue;
    }
    data_027c0280->spriteFree->next = data_027c0280->spriteLists[list].next;
    data_027c0280->spriteFree->x =
        ((group[1] | xBits) & 0xfe00) | ((x + (dx + xBits)) & 0x1ff);
    data_027c0280->spriteFree->y =
        ((group[0] | yBits) & 0xff00) | ((y + (dy + yBits)) & 0xff);
    data_027c0280->spriteFree->unk_08 = tileAdd + group[2];
    data_027c0280->spriteFree->unk_0c = Oam_GetAttrCells(group);
    data_027c0280->spriteFree->unk_0a = attr;
    data_027c0280->spriteLists[list].next = data_027c0280->spriteFree;
    data_027c0280->spriteFree++;
  }
}
