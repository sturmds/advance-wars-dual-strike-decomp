#include "types.h"
#include "game/task.h"

void SpriteTask_Update(void);

TaskScriptCmd gSpriteTaskScript[4] = {
#include "data/main/gSpriteTaskScript.inc.c"
};
