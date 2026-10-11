#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_027c0280 *data_027c0280;

void func_0201eda8(Task *task);
void func_01ff8a2c(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

/* onDestroy of a style 0 pop-up menu: clears the 32 by 20 tiles of layer 2
 * and asks for them to be copied to VRAM. */
void PopupMenu_DestroyStyle0(Task *task)
{
    func_0201eda8(task);
    func_01ff8a2c(data_027c0280->layer2Map, 0, 0, 32, 20, 0);
    Gfx_RequestLayerCopy(2);
}
