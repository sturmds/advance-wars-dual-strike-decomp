#include "types.h"
#include "game/tag_break_task.h"

void func_02059094(int x, int y, int side);

/* Per-frame step of the tag power cut-in: redraws the two func_02059094
 * pieces and scrolls the frame layers on both screens, BG1 by 24 and BG2 by 12
 * pixels a frame. Screen 0 shows the frame 192 pixels further on. */
void TagBreak_ScrollFrame(TagBreakTask *task)
{
    func_02059094(64, 224, 0);
    func_02059094(192, 224, 1);
    task->frameScroll1 += 24;
    task->frameScroll2 += 12;
    TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_00, layer1Bg, task->frameScroll1 + 192);
    TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_04, layer1Bg, task->frameScroll1);
    TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_00, layer2Bg, task->frameScroll2 + 192);
    TAG_BREAK_SET_SCROLL_Y(data_027c0288.unk_04, layer2Bg, task->frameScroll2);
}
