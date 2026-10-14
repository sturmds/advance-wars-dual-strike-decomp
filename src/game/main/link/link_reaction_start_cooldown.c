#include "types.h"
#include "game/task.h"
#include "game/link_reaction_queue.h"

u16 gLinkReactionCooldown;
void Task_ForEachByScript(void*, void*, void*, int);
extern TaskScriptCmd data_0216050c[];
void LinkReaction_ClearOwnDestroyCallback(void);

void LinkReaction_StartCooldown(void) {
  gLinkReactionCooldown = 200;
  Task_ForEachByScript(data_0216050c,
                       (void*)LinkReaction_ClearOwnDestroyCallback,
                       &gLinkReactionCooldown, 200);
}
