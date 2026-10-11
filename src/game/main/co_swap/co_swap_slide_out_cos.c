#include "types.h"
#include "game/co_swap.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Mem_Copy32(const void *src, void *dst, u32 size);
/* Needed to match: the weights narrow to u8 at the call, as if the original
 * prototype took u8. */
void Palette_BlendWithColor(u16 *palette, u16 color, u8 palWeight, u8 colorWeight);
void Palette_Flush(void);
void Task_ResumeScript(Task *task);

extern MenuState gMenuState;

/* Script step of the CO swap cut-in (UnitAction_SwapCo). The front and back
 * CO pictures move apart by up to 128 pixels over 26 frames, and the
 * task->unk_58 title letters stay in place.
 * Frames 1-16 raise blendEva with the frame, fading BG0 in. Frames 17-25
 * blend palette rows 1-3 (saved at the start of gMenuState) toward white,
 * more each frame; frame 26 makes them fully white and resumes the script. */
void CoSwap_SlideOutCos(Task *task)
{
    int i;
    int level;

    CO_SWAP_FRAME(task)++;
    CO_SWAP_OFFSET(task) = Math_Ease(1, 0, 128, CO_SWAP_FRAME(task), 26);
    Sprite_QueueCoPortrait((CO_SWAP_FRONT_X - CO_SWAP_OFFSET(task)) & 0x1ff, CO_SWAP_CO_Y, 0, 16, 2, FALSE);
    Sprite_QueueCoPortrait((CO_SWAP_BACK_X + CO_SWAP_OFFSET(task)) & 0x1ff, CO_SWAP_CO_Y, 0x1000, 0x1c0, 3, FALSE);
    for (i = 0; i < task->unk_58; i++) {
        BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, CO_SWAP_TITLE_Y, 0, 1);
    }
    if (CO_SWAP_FRAME(task) <= 16) {
        data_027c0280->blendEva = CO_SWAP_FRAME(task);
        return;
    }
    switch (CO_SWAP_FRAME(task)) {
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
        CO_SWAP_FRAME(task) = 0;
        level = 16;
        Task_ResumeScript(task);
        break;
    }
    Mem_Copy32(&gMenuState, data_027c0280->paletteBuffer + 0x20, 0x60);
    for (i = 0; i < 3; i++) {
        Palette_BlendWithColor((u16 *)(data_027c0280->paletteBuffer + (i + 1) * 32), 0x7fff,
                               16 - level, level);
    }
    Palette_Flush();
}
