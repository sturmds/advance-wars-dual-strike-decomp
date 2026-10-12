#include "types.h"
#include "game/task.h"
#include "game/tag_gauge_task.h"

extern const TaskScriptCmd
    gTagGaugeScript[]; /* script of the tag firepower gauge */

Task* Task_FindByScript(const TaskScriptCmd* script);

/* TRUE while the gauge script is running and its pop-in step has not run
 * yet; FALSE once it has, or when there is no gauge. */
BOOL TagGauge_IsAnimating(void) {
  TagGaugeTask* task = (TagGaugeTask*)Task_FindByScript(gTagGaugeScript);

  if (task == NULL) {
    return FALSE;
  }
  return task->popInDone == 0;
}
