#include "types.h"
#include "game/Unk_ov005_0235a30c.h"

Ov005Task *Ov005Slide_StartEntryA(Unk_ov005_0235a30c *work, int entryIndex);
Ov005Task *Ov005Slide_StartEntryB(Unk_ov005_0235a30c *work, int entryIndex);

/* 78.8%: the registers of the two loop counters are swapped in the second
 * loop. Creates the task of each entry of both tables. */
void func_ov005_0235ac48(Unk_ov005_0235a30c *work)
{
    int i = 0;
    int j;

    for (;;) {
        if (work->entriesA[i].id == 0) {
            break;
        }
        work->entriesA[i].task = Ov005Slide_StartEntryA(work, i);
        i++;
    }
    j = 0;
    for (;;) {
        if (work->entriesB[j].id == 0) {
            return;
        }
        work->entriesB[j].task = Ov005Slide_StartEntryB(work, j);
        j++;
    }
}
