#include "types.h"
#include "game/tag_break_task.h"

/* Per-frame step of the tag power cut-in: only scrolls the frame. */
void TagBreak_ScrollFrame(TagBreakTask* task) {
  TagBreakTask_ScrollFrame(task);
}
