#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Sound_PlaySe(int id);
void Screen_SetCurrentBoth(void *state);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Mem_Copy32(const void *src, void *dst, u32 size);
/* Needed to match (close): the weights narrow to u8 at each call, as if the
 * original prototype took u8. */
void Palette_BlendWithColor(u16 *palette, u16 color, u8 palWeight, u8 colorWeight);
void Palette_Flush(void);
void Task_ResumeScript(Task *task);

extern u8 gTagChangePalette[];

#define FRAME(task) (*(s16 *)&(task)->unk_4c)

#define SET_BLEND()                                                            \
    data_027c0280->blendMode = BLEND_MODE_ALPHA;                               \
    data_027c0280->blendEva = 0;                                               \
    data_027c0280->blendEvb = 16;                                              \
    data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;                    \
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;                    \
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;                    \
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;                    \
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;                    \
    data_027c0280->blendTopObj = 0;                                            \
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;                 \
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;                 \
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;                 \
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;                 \
    data_027c0280->blendBottomObj = 1;                                         \
    data_027c0280->dispCnt |= 0x100;                                           \
    data_027c0280->dispCnt |= 0x200;                                           \
    data_027c0280->dispCnt |= 0x400;                                           \
    data_027c0280->dispCnt |= 0x800;                                           \
    data_027c0280->dispCnt |= 0x1000

#define WHITEN_PALETTES(palWeight, colorWeight)                                \
    Mem_Copy32(palette, data_027c0280->paletteBuffer + 0x60, 0x60);            \
    for (i = 0; i < 3; i++) {                                                  \
        Palette_BlendWithColor((u16 *)(data_027c0280->paletteBuffer + (i + 3) * 32), 0x7fff, \
                               palWeight, colorWeight);                        \
    }                                                                          \
    Palette_Flush()

/* Script task step. Frame 0 plays sound effect 0xbd and sets both screens to
 * alpha-blend BG0 over BG1-BG3 and OBJ (BG0 not visible yet). Frames 1-16
 * raise eva with the frame, fading BG0 in. Frames 17-25 blend palette rows
 * 3-5 (saved in gTagChangePalette) toward white on both screens, more each
 * frame; frame 26 makes them fully white and resumes the script. Every frame
 * redraws task->unk_58 sprites at y 48.
 * Needed to match: gTagChangePalette goes through a local set after the first
 * Screen_SetCurrentBoth, as in func_02055f84. */
void func_020593c8(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    u8 *palette;                    /* gTagChangePalette, read once for both screens */
    int i;
    int level;

    if (FRAME(task) == 0) {
        Sound_PlaySe(0xbd);
        saved0280 = data_027c0280;
        saved027c = data_027c027c;
        Screen_SetCurrentBoth(data_027c0288.unk_00);
        SET_BLEND();
        Screen_SetCurrentBoth(data_027c0288.unk_04);
        SET_BLEND();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    FRAME(task)++;
    for (i = 0; i < task->unk_58; i++) {
        BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 48, 0, 0);
    }
    if (FRAME(task) <= 16) {
        saved0280 = data_027c0280;
        saved027c = data_027c027c;
        Screen_SetCurrentBoth(data_027c0288.unk_00);
        data_027c0280->blendEva = FRAME(task);
        Screen_SetCurrentBoth(data_027c0288.unk_04);
        data_027c0280->blendEva = FRAME(task);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
        return;
    }
    switch (FRAME(task)) {
    case 17:
        level = 1;
        break;
    case 18:
        level = 2;
        break;
    case 19:
        level = 3;
        break;
    case 20:
        level = 4;
        break;
    case 21:
        level = 5;
        break;
    case 22:
        level = 7;
        break;
    case 23:
        level = 9;
        break;
    case 24:
        level = 11;
        break;
    case 25:
        level = 13;
        break;
    default:
        FRAME(task) = 0;
        level = 16;
        Task_ResumeScript(task);
        break;
    }
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    palette = gTagChangePalette;
    WHITEN_PALETTES(16 - level, level);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    WHITEN_PALETTES(16 - level, level);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
