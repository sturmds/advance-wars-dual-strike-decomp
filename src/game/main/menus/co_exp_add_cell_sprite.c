#include "types.h"
#include "game/menu_state.h"

void *SpriteAnimTask_Start(void *file, int x, int y, int id, int a, int b);

extern MenuState gMenuState;

/* Starts the animation with id 476 from the file at gMenuState + 0x14c4,
 * 32 pixels below (x, y). */
void CoExp_AddCellSprite(int x, int y)
{
    SpriteAnimTask_Start((u8 *)&gMenuState + 0x14c4, x & 0x1ff, (y + 32) & 0xff, 476, 0, 2);
}
