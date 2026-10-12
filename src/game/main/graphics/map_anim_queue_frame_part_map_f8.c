#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;

int MainLoop_GetFrameCount(void);
void GfxQueue_AddKind1(const void* src, void* dst, u32 size);

/* Parts 0-2 of an animation frame are 0xc00 bytes, part 3 is 0xd80. */
#define QUEUE_PART(frame, part)                     \
  do {                                              \
    src = anim->animFrames[frame] + (part) * 0xc00; \
    dst = anim->animDst + (part) * 0xc00;           \
    size = (part) == 3 ? 0xd80 : 0xc00;             \
  } while (0)

/* MapAnim_QueueFramePart for map 0xf8: on a 72-frame cycle, queues
 * animation frames 0, 1, 2 and 1 for 12 game frames each in four parts,
 * one every 3 game frames; frame 1 then stays until the cycle restarts.
 * Does nothing unless data_027c027c is the first screen. Returns the bytes
 * queued; on the other screen the return value is never set. */
int MapAnim_QueueFramePartMapF8(void) {
  Unk_022a7c5c* anim;
  u8* src;
  u8* dst;
  int size;
  int frame;

  anim = data_022a7c5c;
  frame = MainLoop_GetFrameCount();
  if (data_027c027c == data_027c0288.unk_00) {
    size = 0;
    switch (frame % 72) {
      case 1:
        QUEUE_PART(0, 0);
        break;
      case 4:
        QUEUE_PART(0, 1);
        break;
      case 7:
        QUEUE_PART(0, 2);
        break;
      case 10:
        QUEUE_PART(0, 3);
        break;
      case 13:
        QUEUE_PART(1, 0);
        break;
      case 16:
        QUEUE_PART(1, 1);
        break;
      case 19:
        QUEUE_PART(1, 2);
        break;
      case 22:
        QUEUE_PART(1, 3);
        break;
      case 25:
        QUEUE_PART(2, 0);
        break;
      case 28:
        QUEUE_PART(2, 1);
        break;
      case 31:
        QUEUE_PART(2, 2);
        break;
      case 34:
        QUEUE_PART(2, 3);
        break;
      case 37:
        QUEUE_PART(1, 0);
        break;
      case 40:
        QUEUE_PART(1, 1);
        break;
      case 43:
        QUEUE_PART(1, 2);
        break;
      case 46:
        QUEUE_PART(1, 3);
        break;
    }
    if (size != 0) {
      GfxQueue_AddKind1(src, dst, size);
    }
  }
  return size;
}
