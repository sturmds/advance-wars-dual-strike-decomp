#include "types.h"
#include "game/ov003_message_task.h"
#include "game/ov003_unit.h"

#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
const u8* Text_Get(u32 textId);
int strlen(const u8* str);
int Ov003_GetTextWidth(const u8* str);
int Ov003_DrawLetterAlt(u8 ch, u16 x, u16 y);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(Ov003MessageTask* task);

extern s16 gSineTable[];

#define MESSAGE_TOP_TEXT 0x7d000021
#define MESSAGE_BOTTOM_TEXT 0x7d000022
#define MESSAGE_FRAMES 8

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Shows the same two lines as Ov003_ShowTwoLineMessage, at y 832 and 864
 * (sprite y 0x340 and 0x360), for MESSAGE_FRAMES frames. Meanwhile it sets
 * affine matrix 0 to scales that ease like those of Ov003_ShowWarpedMessage.
 * Then it resumes the task's script. Needed to match: the two lines use
 * separate counters and x positions. */
void Ov003_ShowWarpedTwoLineMessage(Ov003MessageTask* task) {
  u16 scaleX;
  u16 scaleY;
  int d3;
  int d2;
  int d1;
  int d0;
  int sinValue;
  int cosValue;
  int topChar;
  int topX;
  int bottomChar;
  int bottomX;

  if (STATE_BYTE_1D7 == 0) {
    task->frames++;
  }
  scaleX = Math_Ease(0, 256, 512, task->frames, MESSAGE_FRAMES);
  scaleY = Math_Ease(0, 256, 2, task->frames, MESSAGE_FRAMES);
  d3 = DIVISOR(scaleY);
  d2 = DIVISOR(scaleX);
  d1 = DIVISOR(scaleY);
  d0 = DIVISOR(scaleX);
  cosValue = gSineTable[64] * 16;
  sinValue = gSineTable[0];
  Oam_SetAffineParams(0, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                      HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
  topX = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_TOP_TEXT)) >> 1);
  for (topChar = 0; topChar < strlen(Text_Get(MESSAGE_TOP_TEXT)); topChar++) {
    topX += Ov003_DrawLetterAlt(Text_Get(MESSAGE_TOP_TEXT)[topChar],
                                topX & 0x1ff, 832);
  }
  bottomX = 128 - (Ov003_GetTextWidth(Text_Get(MESSAGE_BOTTOM_TEXT)) >> 1);
  for (bottomChar = 0; bottomChar < strlen(Text_Get(MESSAGE_BOTTOM_TEXT));
       bottomChar++) {
    bottomX += Ov003_DrawLetterAlt(Text_Get(MESSAGE_BOTTOM_TEXT)[bottomChar],
                                   bottomX & 0x1ff, 864);
  }
  if (task->frames == MESSAGE_FRAMES) {
    task->frames = 0;
    Task_ResumeScript(task);
  }
}
