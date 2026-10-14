#include "types.h"
#include "game/tab_list.h"
#include "game/oam_cells.h"

extern TabListView gMenuState;
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
int TabList_GetPageCount(int tab);

/* Queues "page / pageCount" as three digit sprites. The digit glyph for n is
 * at tile 0x80 + 2 * n; 0x2094 is the slash. */
void TabList_DrawPageNumber(void) {
  int page = gMenuState.pageOfTab[gMenuState.tab];
  int pageCount = TabList_GetPageCount(gMenuState.tab);

  Sprite_QueueAt(0, 200, 32, &data_027c0028, ((page + 1) * 2 + 0x80) | 0x2000,
                 0);
  Sprite_QueueAt(0, 208, 32, &data_027c0028, 0x2094, 0);
  Sprite_QueueAt(0, 216, 32, &data_027c0028, (pageCount * 2 + 0x80) | 0x2000,
                 0);
}
