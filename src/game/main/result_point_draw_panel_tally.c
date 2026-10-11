#include "types.h"
#include "game/Unk_0229c734.h"

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void ResultPoint_DrawTallyPopIn(int x, int y, int points, int tally);

extern u8 data_027c0060[];
extern u8 data_027c01ac[];

#define SCORES data_0229c734

/* Like ResultPoint_DrawPanel, but draws the number with ResultPoint_DrawTallyPopIn, from
 * SCORES->tally and the task's tallyPopFrame. */
void ResultPoint_DrawPanelTally(int x, int y, ResultPointTask *task)
{
    /* needed to match: y moved down once, cast to u8 at each sprite call */
    y += 8;
    Sprite_QueueAt(6, x & 0x1ff, (u8)y, data_027c01ac, (SCORES->paletteSlot + 1) << 12, SCORES->partTile[3]);
    ResultPoint_DrawTallyPopIn(x + 80, y, SCORES->tally, task->tallyPopFrame);
    Sprite_QueueAt(6, (x + 120) & 0x1ff, (u8)y, data_027c0060, (SCORES->paletteSlot + 1) << 12,
                   SCORES->partTile[3] + 72);
}
