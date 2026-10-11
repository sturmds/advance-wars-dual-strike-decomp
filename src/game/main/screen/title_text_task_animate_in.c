#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const s16 gSineTable[];            /* sine table, 256 steps per turn */

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
const u8 *Text_Get(u32 textId);
int Text_GetStringWidth(const u8 *str);
int func_02039f20(const u8 *str);
int strlen(const u8 *str);       /* string length */
int SpriteFont_QueueChar(int ch, u16 x, u16 y, int attr, u16 firstTile, u8 list);
int CaptionLetter_GetAdvance(int ch, u16 x, u16 y, int attr, u16 firstTile, u8 list);
int HwDiv32(int num, int den);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void func_02032b14(void);
void Task_ResumeScript(Task *task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)

#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

#define DIV_SCALE(v, s) HwDiv32(v, (s) != 0 ? (s) : 2)

#define TEXT_UPPER 0x7d00001e          /* slides up from the main screen to the sub screen */
#define TEXT_LOWER 0x7d00001f          /* zooms in letter by letter on the sub screen */

/* OAM attribute 0 bits: affine sprite drawn at double size. */
#define OAM_AFFINE_DOUBLE 0x300

/* OAM attribute 1: affine parameter set in bits 9-13. */
#define OAM_AFFINE_INDEX(n) ((n) << 9)

/* Task step, 32 frames. On the sub screen layers 0 and 1 slide in from
 * opposite sides. TEXT_UPPER moves up from y 80 on the main screen and
 * continues on the sub screen until it leaves at the top. The letters of
 * TEXT_LOWER appear one after another over 24 frames on the sub screen,
 * each shrinking from twice its size in 8 frames. Then the script goes on. */
void TitleTextTask_AnimateIn(Task *task)
{
    /* needed to match: this declaration order */
    Unk_027c0280 *savedGfx2;
    Unk_02080440 *savedState2;
    Unk_02080440 *savedState1;
    Unk_027c0280 *savedGfx3;
    int start;
    int i;
    int offset;
    Unk_02080440 *savedState3;
    int x;
    Unk_027c0280 *savedGfx1;
    int scale;
    int y;

    FRAME(task)++;
    offset = Math_Ease(4, 256, 0, FRAME(task), 32);
    savedGfx1 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState1 = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = -offset;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = offset;
    Screen_SetCurrent(savedState1);
    Screen_SetCurrentGfx(savedGfx1);

    y = Math_Ease(4, 80, -112, FRAME(task), 32);
    savedGfx2 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState2 = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (y > -16) {
        x = 128 - (func_02039f20(Text_Get(TEXT_UPPER)) >> 1);
        for (i = 0; i < strlen(Text_Get(TEXT_UPPER)); i++) {
            x += SpriteFont_QueueChar(Text_Get(TEXT_UPPER)[i], x & 0x1ff, y & 0xff, 0x1000, 0, 6);
        }
    }
    Screen_SetCurrent(savedState2);
    Screen_SetCurrentGfx(savedGfx2);

    savedGfx3 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState3 = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    /* y on the sub screen, 192 lines above the main screen. */
    if (y + 192 < 192) {
        x = 128 - (func_02039f20(Text_Get(TEXT_UPPER)) >> 1);
        for (i = 0; i < strlen(Text_Get(TEXT_UPPER)); i++) {
            x += SpriteFont_QueueChar(Text_Get(TEXT_UPPER)[i], x & 0x1ff, (y + 192) & 0xff, 0x1000, 0, 6);
        }
    }
    x = 128 - (Text_GetStringWidth(Text_Get(TEXT_LOWER)) >> 1);
    for (i = 0; i < strlen(Text_Get(TEXT_LOWER)); i++) {
        start = Math_Ease(0, 0, 24, i, strlen(Text_Get(TEXT_LOWER)) - 1);
        if (FRAME(task) >= start) {
            if (FRAME(task) < start + 8) {
                scale = Math_Ease(0, 512, 256, FRAME(task) - start, 8);
                Oam_SetAffineParams(i + 1, DIV_SCALE(COS0 << 4, scale), DIV_SCALE(-SIN0 << 4, scale),
                                    DIV_SCALE(SIN0 << 4, scale), DIV_SCALE(COS0 << 4, scale));
                x += CaptionLetter_GetAdvance(Text_Get(TEXT_LOWER)[i], (x & 0x1ff) | OAM_AFFINE_INDEX(i + 1), OAM_AFFINE_DOUBLE | 72, 0x1000, 0, 6);
            } else {
                x += CaptionLetter_GetAdvance(Text_Get(TEXT_LOWER)[i], x & 0x1ff, 72, 0x1000, 0, 6);
            }
        }
    }
    Screen_SetCurrent(savedState3);
    Screen_SetCurrentGfx(savedGfx3);

    if (FRAME(task) == 32) {
        FRAME(task) = 0;
        func_02032b14();
        Task_ResumeScript(task);
    }
}
