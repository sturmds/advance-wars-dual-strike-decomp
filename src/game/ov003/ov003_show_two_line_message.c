#include "types.h"
#include "game/ov003_unit.h"

typedef struct Ov003MessageTask {
    u8 pad_00[0x4c];
    s16 frames;                        /* frames shown, counted while the byte at 0x1d7 of the state is 0 */
} Ov003MessageTask;

#define STATE_BYTE_1D7 (((u8 *)&data_ov003_02364928->unk_1d6)[1])

#define MESSAGE_TOP_TEXT 0x7d000021
#define MESSAGE_BOTTOM_TEXT 0x7d000022
#define MESSAGE_FRAMES 24

const u8 *Text_Get(u32 textId);
int func_021247ac(const u8 *str);                      /* strlen */
int func_ov003_02363d5c(const u8 *str);                /* width of a string in pixels (suggested name: Ov003_GetTextWidth) */
int func_ov003_02363c88(u8 ch, u16 x, u16 y);          /* draws one character, returns its width (suggested name: Ov003_DrawChar) */
void Task_ResumeScript(Ov003MessageTask *task);

/* Draws two lines of text (0x7d000021 at y 64, 0x7d000022 at y 96), each
 * centred on x 128, one character at a time. After MESSAGE_FRAMES frames it
 * resets the counter and resumes the task's script. Needed to match: the two
 * lines use separate counters and x positions. */
void Ov003_ShowTwoLineMessage(Ov003MessageTask *task)
{
    int topChar;
    int topX;
    int bottomChar;
    int bottomX;

    if (STATE_BYTE_1D7 == 0) {
        task->frames++;
    }
    topX = 128 - (func_ov003_02363d5c(Text_Get(MESSAGE_TOP_TEXT)) >> 1);
    for (topChar = 0; topChar < func_021247ac(Text_Get(MESSAGE_TOP_TEXT)); topChar++) {
        topX += func_ov003_02363c88(Text_Get(MESSAGE_TOP_TEXT)[topChar], topX & 0x1ff, 64);
    }
    bottomX = 128 - (func_ov003_02363d5c(Text_Get(MESSAGE_BOTTOM_TEXT)) >> 1);
    for (bottomChar = 0; bottomChar < func_021247ac(Text_Get(MESSAGE_BOTTOM_TEXT)); bottomChar++) {
        bottomX += func_ov003_02363c88(Text_Get(MESSAGE_BOTTOM_TEXT)[bottomChar], bottomX & 0x1ff, 96);
    }
    if (task->frames == MESSAGE_FRAMES) {
        task->frames = 0;
        Task_ResumeScript(task);
    }
}
