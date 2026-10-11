#include "types.h"
#include "game/force_rank_screen.h"

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);

extern u8 data_027c0028;

/* Queues a mark over the grid entry of each skill chosen in the open slots,
 * at the button positions of ForceRankScreen_AddButtons. */
void ForceRankScreen_MarkChosenSkills(void)
{
    int i;
    int slotCount;
    int row;
    int column;
    int skill;

    slotCount = gForceRankScreen->rank;
    if (slotCount > 4) {
        slotCount = 4;
    }
    for (i = 0; i < slotCount; i++) {
        skill = gForceRankScreen->chosenSkills[i];
        if (skill != 0) {
            column = Ov000_gForceRankTable[skill].column;
            for (row = 0; row < gForceRankScreen->columns[column].count; row++) {
                /* Needed to match: reads chosenSkills[i] again instead of skill. */
                if (gForceRankScreen->columns[column].skills[row] == gForceRankScreen->chosenSkills[i]) {
                    Sprite_QueueAt(1, ((column - 1) * 24) & 0x1ff, (row * 16 + 64) & 0xff, &data_027c0028,
                                   0x2058, 0);
                    break;
                }
            }
        }
    }
}
