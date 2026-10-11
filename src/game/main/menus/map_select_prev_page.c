#include "types.h"
#include "game/event_queue.h"
#include "game/map_select.h"
#include "game/menu_button.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int func_0207bd80(void);
void MapSelect_TurnToPage(u32 page);

/* Pages of the current list. */
#define PAGE_COUNT() (HwDiv32(LIST(TAB()).mapCount - 1, ROWS * COLS) + 1)

/* Y key and the touch button at (224, 96) on the map select screen: turns
 * the current list back one page, from the first page to the last. In link
 * play the turn goes out as event 3 instead. */
void MapSelect_PrevPage(MenuButton *button)
{
    int page;

    page = PAGE();
    /* Needed to match: an unsigned compare, as in MapSelect_TurnToPage. */
    if ((u32)PAGE_COUNT() >= 2) {
        if (page == 0) {
            page = PAGE_COUNT() - 1;
        } else {
            page--;
        }
    }
    if (page == PAGE()) {
        return;
    }
    if (func_0207bd80() == 1) {
        EventQueue_Push(3, (u8)page);
    } else {
        MapSelect_TurnToPage(page);
    }
}
