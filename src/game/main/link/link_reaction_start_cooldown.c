#include "types.h"

/* also declared in src/game/main/func_0208109c.c and 2 more */
u16 gLinkReactionCooldown;
void Task_ForEachByScript(void*, void*, void*, int);
extern u8 data_0216050c[];
void LinkReaction_ClearOwnDestroyCallback(void);

void LinkReaction_StartCooldown(void) {
  gLinkReactionCooldown = 200;
  Task_ForEachByScript(data_0216050c,
                       (void*)LinkReaction_ClearOwnDestroyCallback,
                       &gLinkReactionCooldown, 200);
}
