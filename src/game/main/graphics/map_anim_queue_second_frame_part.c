#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"

int MainLoop_GetFrameCount(void);
void GfxQueue_AddKind1(const void* src, void* dst, u32 size);

/* Parts 0-3 of an animation frame are 0x1000 bytes, part 4 is 0x400. */
#define QUEUE_PART(frame, part)                       \
  do {                                                \
    src = anim->anim2Frames[frame] + (part) * 0x1000; \
    dst = anim->anim2Dst + (part) * 0x1000;           \
    size = (part) == 4 ? 0x400 : 0x1000;              \
  } while (0)

/* Like MapAnim_QueueFramePart for the second animation, on a 60-frame
 * cycle: shows animation frames 0, 1, 2, 1 for 15 game frames each, and
 * queues each one in five parts, one every 3 game frames. Does nothing
 * unless data_027c027c is the first screen. Returns the bytes queued; on
 * the other screen the return value is never set (the caller ignores it). */
int MapAnim_QueueSecondFramePart(void) {
  Unk_022a7c5c* anim;
  u8* src;
  u8* dst;
  int size;
  int frame;

  anim = data_022a7c5c;
  frame = MainLoop_GetFrameCount();
  if (data_027c027c == data_027c0288.unk_00) {
    size = 0;
    switch (frame % 60) {
      case 0:
        QUEUE_PART(0, 0);
        break;
      case 3:
        QUEUE_PART(0, 1);
        break;
      case 6:
        QUEUE_PART(0, 2);
        break;
      case 9:
        QUEUE_PART(0, 3);
        break;
      case 12:
        QUEUE_PART(0, 4);
        break;
      case 15:
        QUEUE_PART(1, 0);
        break;
      case 18:
        QUEUE_PART(1, 1);
        break;
      case 21:
        QUEUE_PART(1, 2);
        break;
      case 24:
        QUEUE_PART(1, 3);
        break;
      case 27:
        QUEUE_PART(1, 4);
        break;
      case 30:
        QUEUE_PART(2, 0);
        break;
      case 33:
        QUEUE_PART(2, 1);
        break;
      case 36:
        QUEUE_PART(2, 2);
        break;
      case 39:
        QUEUE_PART(2, 3);
        break;
      case 42:
        QUEUE_PART(2, 4);
        break;
      case 45:
        QUEUE_PART(1, 0);
        break;
      case 48:
        QUEUE_PART(1, 1);
        break;
      case 51:
        QUEUE_PART(1, 2);
        break;
      case 54:
        QUEUE_PART(1, 3);
        break;
      case 57:
        QUEUE_PART(1, 4);
        break;
    }
    if (size != 0) {
      GfxQueue_AddKind1(src, dst, size);
    }
  }
  return size;
}
