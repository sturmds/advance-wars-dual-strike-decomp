#include "types.h"
#include "game/quad_sprite_task.h"

#define GXFIFO_POLYGON_ATTR ((volatile u32*)0x040004a4)

void Gx_DrawTexturedQuad(int x, int y, int z, int w, int h, int sizeS,
                         int sizeT, u16 texOffset, u8 paletteOffset, u8 flipX,
                         u8 flipY);

/* Draws each quad of the task's current frame, scaled and placed at the
 * task's position less its scroll. With cull set, quads wholly
 * off the 256 x 192 screen are skipped. */
void AnimSpriteTask_Draw(QuadSpriteTask* task) {
  const QuadSpritePart* part = task->frame;
  s16 x;
  s16 y;
  s16 w;
  s16 h;

  /* light 0, front and back faces, opaque */
  *GXFIFO_POLYGON_ATTR = 0x001f00c1;
  for (; part->width != 0; part++) {
    x = part->x;
    if (x & 0x100) {
      x |= 0xff00;
    }
    y = part->y;
    if (y & 0x100) {
      y |= 0xff00;
    }
    x = task->x + (s16)((x * task->scaleX) >> 8);
    y = task->y + (s16)((y * task->scaleY) >> 8);
    if (task->scrollX != NULL) {
      x -= *task->scrollX;
    }
    if (task->scrollY != NULL) {
      y -= *task->scrollY;
    }
    w = (part->width * task->scaleX) >> 8;
    h = (part->height * task->scaleY) >> 8;
    if (task->cull) {
      if (x < -w || x >= 256 || y < -h || y >= 192) {
        continue;
      }
    }
    Gx_DrawTexturedQuad(x, y, (s16)(task->z + part->z), w, h, part->sizeS,
                        part->sizeT, task->tileBase + part->tile,
                        task->paletteBase + part->palette, part->flipX,
                        part->flipY);
  }
}
