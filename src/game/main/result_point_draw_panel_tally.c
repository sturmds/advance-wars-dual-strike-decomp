#include "types.h"
#include "game/Unk_0229c734.h"

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void func_02039560(int x, int y, int points, int tally);

extern u8 data_027c0060[];
extern u8 data_027c01ac[];

#define SCORES data_0229c734

void func_02039310(int x, int y, ResultPointTask *task)
{
    u8 spriteY = y + 8;

    Sprite_QueueAt(6, x & 0x1ff, spriteY, data_027c01ac, (SCORES->paletteSlot + 1) << 12, SCORES->partTile[3]);
    func_02039560(x + 80, y + 8, SCORES->tally, task->unk_34);
    Sprite_QueueAt(6, (x + 120) & 0x1ff, spriteY, data_027c0060, (SCORES->paletteSlot + 1) << 12,
                   SCORES->partTile[3] + 72);
}
