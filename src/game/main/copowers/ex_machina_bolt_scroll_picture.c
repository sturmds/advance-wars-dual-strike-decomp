#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ex_machina_bolt_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void Bg_SetScroll(int layer, int scrollX, int scrollY);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step of gExMachinaBoltScript: scrolls layer 0 (the bolt picture)
 * so that it sits on the bolt's tile, whichever way the map view is
 * scrolled. The offsets 40 and 208 put the picture's strike point on the
 * tile. */
void ExMachinaBolt_ScrollPicture(ExMachinaBoltTask *task)
{
    Bg_SetScroll(0, 40 - (task->tileX * 16 - STATE->unk_1c4->scrollX),
                  208 - (task->tileY * 16 - STATE->unk_1c4->scrollY));
}
