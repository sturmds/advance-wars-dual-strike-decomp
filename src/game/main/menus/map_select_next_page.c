#include "types.h"
#include "game/map_select.h"
#include "game/event_queue.h"
#include "game/math.h"

int Link_IsActive(void);
void MapSelect_TurnToPage(u32 page);

/* Pages in the current list. */
#define PAGE_COUNT() (HwDiv32(LIST(TAB()).mapCount - 1, ROWS * COLS) + 1)

/* Turns the current list to its next page, wrapping from the last page to
 * the first. In a link game (Link_IsActive() == 1) it sends the page change
 * as a link message instead. */
void MapSelect_NextPage(void) {
  u32 page = PAGE();

  if ((u32)PAGE_COUNT() >= 2) {
    if (page == PAGE_COUNT() - 1) {
      page = 0;
    } else {
      page++;
    }
  }
  if (page == PAGE()) {
    return;
  }
  if (Link_IsActive() == 1) {
    EventQueue_Push(EVENT_MAP_SELECT_PAGE, (u8)page);
  } else {
    MapSelect_TurnToPage(page);
  }
}
