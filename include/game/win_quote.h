#ifndef GAME_WIN_QUOTE_H
#define GAME_WIN_QUOTE_H

#include "types.h"
#include "game/task.h"
#include "game/Unk_022a7a9c.h"

/* The win quote screen, a task script with its steps at 0x0215a0bc-0x0215a0dc:
 * func_02033088 loads the COs of result record task->unk_64,
 * WinQuote_SlideIn slides them in and prints the quote, WinQuote_WaitText
 * and WinQuote_WaitKeys wait. */

void func_02002620(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);

/* Task.unk_64 is a u16 in task.h, but this task reads it as signed. */
#define WIN_QUOTE_RECORD(task) (*(s16 *)&(task)->unk_64)

/* Draws the CO sprites that func_02033088 loaded: the front CO, plus the
 * back CO when the record has a tag partner. */
static inline void WinQuote_DrawCos(Task *task)
{
    if (data_022a7aba[WIN_QUOTE_RECORD(task)].unk_00 & 0x7f) {
        func_02002620(64, 160, 0x5800, 0x224, 5, FALSE);
        func_02002620(192, 160, 0x6800, 0x3d4, 6, TRUE);
        Sprite_QueueDefaultTile(96, 88, 0x4000, 0x1dc, 6);
        Sprite_QueueDefaultTile(96, 128, 0x4000, 0x1e8, 6);
    } else {
        func_02002620(64, 160, 0x5800, 0x224, 5, FALSE);
        Sprite_QueueDefaultTile(96, 128, 0x4000, 0x1dc, 6);
    }
}

#endif
