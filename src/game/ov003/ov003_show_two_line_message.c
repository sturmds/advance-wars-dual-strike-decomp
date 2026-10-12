#include "types.h"
#include "game/ov003_message_task.h"
#include "game/ov003_unit.h"

#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

#define MESSAGE_TOP_TEXT 0x7d000021
#define MESSAGE_BOTTOM_TEXT 0x7d000022
#define MESSAGE_FRAMES 24

const u8* Text_Get(u32 textId);
int strlen(const u8* str); /* strlen */
int Ov003_GetTextWidth(
    const u8*
        str); /* width of a string in pixels (suggested name: Ov003_GetTextWidth) */
int Ov003_DrawLetterAlt(
    u8 ch, u16 x,
    u16 y); /* draws one character, returns its width (suggested name: Ov003_DrawChar) */
void Task_ResumeScript(Ov003MessageTask* task);

/* Draws two lines of text (0x7d000021 at y 64, 0x7d000022 at y 96), each
 * centred on x 128, one character at a time. After MESSAGE_FRAMES frames it
 * resets the counter and resumes the task's script. Needed to match: the two
 * lines use separate counters and x positions. */
void Ov003_ShowTwoLineMessage(Ov003MessageTask* task) {
  int topChar;
  int topX;
  int bottomChar;
  int bottomX;

  if (STATE_BYTE_1D7 == 0) {
    task->frames++;
  }
  topX = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_TOP_TEXT)) >> 1);
  for (topChar = 0; topChar < strlen(Text_Get(MESSAGE_TOP_TEXT)); topChar++) {
    topX += Ov003_DrawLetterAlt(Text_Get(MESSAGE_TOP_TEXT)[topChar],
                                topX & 0x1ff, 64);
  }
  bottomX = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_BOTTOM_TEXT)) >> 1);
  for (bottomChar = 0; bottomChar < strlen(Text_Get(MESSAGE_BOTTOM_TEXT));
       bottomChar++) {
    bottomX += Ov003_DrawLetterAlt(Text_Get(MESSAGE_BOTTOM_TEXT)[bottomChar],
                                   bottomX & 0x1ff, 96);
  }
  if (task->frames == MESSAGE_FRAMES) {
    task->frames = 0;
    Task_ResumeScript(task);
  }
}
