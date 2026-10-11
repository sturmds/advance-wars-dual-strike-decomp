#ifndef GAME_WIN_QUOTE_H
#define GAME_WIN_QUOTE_H

#include "types.h"
#include "game/task.h"
#include "game/Unk_022a7a9c.h"

/* The win quote screen, a task script with its steps at 0x0215a0bc-0x0215a0dc:
 * WinQuote_Init loads the COs of result record task->unk_64,
 * WinQuote_SlideIn slides them in and prints the quote, WinQuote_WaitText
 * and WinQuote_WaitKeys wait. */

void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);

/* Result record index, kept in Task.unk_64. */
#define WIN_QUOTE_RECORD(task) ((task)->unk_64)

/* Task.unk_4c is a pointer in task.h; WinQuote_SlideIn keeps its frame
 * count in the low halfword. */
#define WIN_QUOTE_FRAME(task) (*(s16 *)&(task)->unk_4c)

/* Draws the CO sprites that WinQuote_Init loaded: the front CO, plus the
 * back CO when the record has a tag partner. */
static inline void WinQuote_DrawCos(Task *task)
{
    if (data_022a7aba[WIN_QUOTE_RECORD(task)].unk_00 & 0x7f) {
        Sprite_QueueCoPortrait(64, 160, 0x5800, 0x224, 5, FALSE);
        Sprite_QueueCoPortrait(192, 160, 0x6800, 0x3d4, 6, TRUE);
        Sprite_QueueDefaultTile(96, 88, 0x4000, 0x1dc, 6);
        Sprite_QueueDefaultTile(96, 128, 0x4000, 0x1e8, 6);
    } else {
        Sprite_QueueCoPortrait(64, 160, 0x5800, 0x224, 5, FALSE);
        Sprite_QueueDefaultTile(96, 128, 0x4000, 0x1dc, 6);
    }
}

#endif
