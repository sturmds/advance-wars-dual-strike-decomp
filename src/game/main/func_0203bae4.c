#include "types.h"
#include "game/task.h"

/* y is u8 here: the original narrows it at the call, and an explicit
 * `y & 0xff` gets hoisted out of the column loop instead. */
void Sprite_QueueAt(int list, int x, u8 y, void *data, int attr, int tile);
int Player_GetCoIconTile(int player, int type);
int func_020c7614(int player, int unitType);
int func_020c7580(int player, int unitType);
BOOL func_0203bf2c(int x, int width);
void ResultStats_DrawCount(int x, int y, int value);

extern const u8 data_0215a51c[];               /* unit types, 7 rows of 4; 0 leaves a cell empty */
extern u8 data_027c0040;

/* Draws the unit grid of a stats page for player task->unk_66, moved right
 * by `offset` pixels: each unit's icon and a count. task->unk_68 picks the
 * count (0: func_020c7614, 1: func_020c7580); units with a count of 0 are
 * left out. */
void func_0203bae4(int offset, Task *task)
{
    int row;
    int col;
    int x;
    u8 type;

    for (row = 0; row < 7; row++) {
        for (col = 0, x = 25; col < 4; x += 54, col++) {
            type = data_0215a51c[row * 4 + col];
            if (type == 0) {
                continue;
            }
            if (task->unk_68 == 0 && func_020c7614(task->unk_66, type) == 0) {
                continue;
            }
            if (task->unk_68 == 1 && func_020c7580(task->unk_66, type) == 0) {
                continue;
            }
            if (!func_0203bf2c(offset + x, 40)) {
                continue;
            }
            Sprite_QueueAt(0, (offset + x) & 0x1ff, 48 + row * 16, &data_027c0040, (task->unk_66 + 1) << 12,
                           Player_GetCoIconTile(task->unk_66, type) + 134);
            if (task->unk_68 == 0) {
                ResultStats_DrawCount(offset + x + 16, 48 + row * 16, func_020c7614(task->unk_66, type));
            } else {
                ResultStats_DrawCount(offset + x + 16, 48 + row * 16, func_020c7580(task->unk_66, type));
            }
        }
    }
}
