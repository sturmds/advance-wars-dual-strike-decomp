#include "types.h"
#include "game/ov003_gfx.h"
#include "game/screen_other_map.h"
#include "game/ov003_message_task.h"
#include "game/ov003_unit.h"

#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Mem_Copy32(const void* src, void* dst, u32 size);
const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int Ov003_GetTextWidth(const u8* str);
int Ov003_DrawLetter(u8 ch, u16 x, u16 y);
void Task_ResumeScript(Ov003MessageTask* task);

#define MESSAGE_TEXT 0x7d000024
#define MESSAGE_FRAMES 60
#define SLIDE_FRAMES 16

/* Shows the text 0x7d000024 centred, at sprite y 80 plus a vertical offset.
 * Frame 1 copies the 0x2000-byte block from Ov003_LoadObjGraphics into object
 * VRAM. For the first SLIDE_FRAMES frames the text slides in, easing the offset
 * from -112 to 0. Then the offset follows a table of small bounces (0 to -10,
 * written as a switch to match) until frame 44, and stays 0. After
 * MESSAGE_FRAMES frames it resumes the task's script. */
void Ov003_ShowBouncingMessage(Ov003MessageTask* task) {
  int i;
  int x;
  int offset;

  if (STATE_BYTE_1D7 == 0) {
    task->frames++;
  }
  if (task->frames == 1) {
    Mem_Copy32(data_ov003_02365b0c, data_027c0280->objVram, 0x2000);
  }
  x = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_TEXT)) >> 1);
  if (task->frames <= SLIDE_FRAMES) {
    offset = Math_Ease(1, -112, 0, task->frames, SLIDE_FRAMES);
  } else {
    switch (task->frames - SLIDE_FRAMES) {
      case 1:
        offset = 0;
        break;
      case 2:
        offset = -2;
        break;
      case 3:
        offset = -4;
        break;
      case 4:
        offset = -6;
        break;
      case 5:
        offset = -8;
        break;
      case 6:
        offset = -10;
        break;
      case 7:
        offset = -8;
        break;
      case 8:
        offset = -6;
        break;
      case 9:
        offset = -4;
        break;
      case 10:
        offset = -2;
        break;
      case 11:
        offset = 0;
        break;
      case 12:
        offset = -2;
        break;
      case 13:
        offset = -4;
        break;
      case 14:
        offset = -6;
        break;
      case 15:
        offset = -8;
        break;
      case 16:
        offset = -6;
        break;
      case 17:
        offset = -4;
        break;
      case 18:
        offset = -2;
        break;
      case 19:
        offset = 0;
        break;
      case 20:
        offset = -2;
        break;
      case 21:
        offset = -4;
        break;
      case 22:
        offset = -6;
        break;
      case 23:
        offset = -4;
        break;
      case 24:
        offset = -2;
        break;
      case 25:
        offset = 0;
        break;
      case 26:
        offset = -2;
        break;
      case 27:
        offset = -4;
        break;
      case 28:
        offset = -2;
        break;
      default:
        offset = 0;
        break;
    }
  }
  for (i = 0; i < strlen(Text_Get(MESSAGE_TEXT)); i++) {
    x += Ov003_DrawLetter(Text_Get(MESSAGE_TEXT)[i], x & 0x1ff,
                          (offset + 80) & 0xff);
  }
  if (task->frames == MESSAGE_FRAMES) {
    task->frames = 0;
    Task_ResumeScript(task);
  }
}
