#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Clears layer 1, where the pop-up menu box was drawn, and runs the
 * game state's unk_38 hook. The callers pass four values this ignores. */
void PopupMenu_ClearBox(int unused0, int unused1, int unused2, int unused3)
{
    func_01ff8c84(1);
    Gfx_RequestLayerCopy(1);
    if (data_027c027c->unk_50[data_027c0284]->unk_38 != NULL) {
        data_027c027c->unk_50[data_027c0284]->unk_38();
    }
}
