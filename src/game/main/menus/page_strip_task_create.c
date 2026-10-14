#include "types.h"
#include "game/page_strip.h"
#include "game/task.h"

extern TaskScriptCmd data_0215ae90[];

/* Starts the page strip draw task as a child of the task that calls this
 * and gives it that task's strip fields. */
void PageStripTask_Create(PageStripTask* parent) {
  PageStripTask* strip = (PageStripTask*)Task_Create(data_0215ae90, parent);

  strip->cursor = parent->cursor;
  strip->unk_66 = parent->unk_66;
  strip->mapId =
      *(s16*)&parent
           ->mapId; /* needed to match: the original reads it as signed */
  strip->count = parent->count;
  strip->pages = parent->pages;
}
