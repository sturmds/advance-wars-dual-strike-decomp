#include "types.h"
#include "game/ov003_gfx.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

extern Unk_027c0280* data_027c0280;

const void* Text_Get(u32 textId);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Task_ResumeScript(void* task);
int strlen(const char* str);
int Ov003_GetTextWidth(const u8* str);
int Ov003_DrawLetterAlt(int ch, u16 x, u16 y);

/* Task step: while Ov003State.unk_1d7 is 0, counts frames up to 60. On frame 1
 * it copies 0x2000 bytes from data_ov003_02365b0c to object VRAM. If
 * data_02160abc->unk_0a equals the row of the current slot, it draws text
 * 0x7d000023 centred each frame at y 80, shifted by 6 pixels in a different
 * direction on each of frames 2 to 9 (a shake). At frame 60 it resumes the
 * task script. */
void Ov003_ShakeTextStep(Unk_ov003_0235dd38* obj) {
  int i;
  int x;
  int y;

  if (data_ov003_02364928->unk_1d7 == 0) {
    obj->frame++;
  }
  if (obj->frame == 1) {
    Mem_Copy32(data_ov003_02365b0c, data_027c0280->objVram + 0xb1c0, 0x2000);
  }
  if (data_02160abc->unk_0a ==
      data_ov003_02364928->slots[data_ov003_02364928->currentSlot].row) {
    x = 0;
    y = 0;
    if (obj->frame <= 9) {
      switch (obj->frame) {
        case 1:
          x = 0;
          y = 0;
          break;
        case 2:
          x = 6;
          y = -6;
          break;
        case 3:
          x = -6;
          y = 6;
          break;
        case 4:
          x = 6;
          y = 6;
          break;
        case 5:
          x = -6;
          y = -6;
          break;
        case 6:
          x = 6;
          break;
        case 7:
          y = -6;
          break;
        case 8:
          x = -6;
          break;
        case 9:
          y = 6;
          break;
      }
    }
    x += (128 - (Ov003_GetTextWidth(Text_Get(0x7d000023)) >> 1));
    for (i = 0; i < strlen(Text_Get(0x7d000023)); i++) {
      x += Ov003_DrawLetterAlt(((const u8*)Text_Get(0x7d000023))[i], x & 0x1ff,
                               (y + 80) & 0xff);
    }
  }
  if (obj->frame == 60) {
    obj->frame = 0;
    Task_ResumeScript(obj);
  }
}
