#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_027c0280 *data_027c0280;

void PopupMenu_DestroyCommon(Task *task);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

/* onDestroy of a style 0 pop-up menu: clears the 32 by 20 tiles of layer 2
 * and asks for them to be copied to VRAM. */
void PopupMenu_DestroyStyle0(Task *task)
{
    PopupMenu_DestroyCommon(task);
    Bg_FillRect(data_027c0280->layer2Map, 0, 0, 32, 20, 0);
    Gfx_RequestLayerCopy(2);
}
