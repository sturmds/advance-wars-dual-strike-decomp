#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ex_machina_bolt_task.h"

void Bg_SetScroll(int layer, int scrollX, int scrollY);

/* Script step of gExMachinaBoltScript: scrolls layer 0 (the bolt picture)
 * so that it sits on the bolt's tile, whichever way the map view is
 * scrolled. The offsets 40 and 208 put the picture's strike point on the
 * tile. */
void ExMachinaBolt_ScrollPicture(ExMachinaBoltTask* task) {
  Bg_SetScroll(0, 40 - (task->tileX * 16 - MAP_STATE->unk_1c4->scrollX),
               208 - (task->tileY * 16 - MAP_STATE->unk_1c4->scrollY));
}
