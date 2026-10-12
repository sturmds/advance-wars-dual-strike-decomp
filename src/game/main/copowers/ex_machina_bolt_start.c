#include "types.h"
#include "game/task.h"
#include "game/ex_machina_bolt_task.h"

extern const TaskScriptCmd gExMachinaBoltScript[];

/* Starts the bolt that hits one tile: a child task of the Ex Machina task
 * running gExMachinaBoltScript. */
void ExMachinaBolt_Start(int tileX, int tileY, Task* parent) {
  ExMachinaBoltTask* bolt =
      (ExMachinaBoltTask*)Task_CreateChild(gExMachinaBoltScript, parent);

  bolt->tileX = tileX;
  bolt->tileY = tileY;
}
