#include "types.h"
#include "game/menu_state.h"

struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

extern MenuState gMenuState;

/* Starts the animation with id 476 from the file at MenuState.coExp.cellSprites,
 * 32 pixels below (x, y). Needed to match: the offset is added to
 * gMenuState's address at run time. */
void CoExp_AddCellSprite(int x, int y) {
  SpriteAnimTask_Start(
      (u8*)&gMenuState + offsetof(MenuState, coExp.cellSprites), x & 0x1ff,
      (y + 32) & 0xff, 476, 0, 2);
}
