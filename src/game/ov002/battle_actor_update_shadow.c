#include "types.h"
#include "game/battle_actor.h"

void Task_Destroy(void *task);
s16 func_ov002_0235867c(BattleActor *actor);
int func_ov002_02358600(BattleActor *actor);

/* Destroys the actor's shadow task once the shadow is done, otherwise moves it
 * with the actor. The second NULL check is needed to match. */
void BattleActor_UpdateShadow(BattleActor *actor)
{
    BattleShadow *shadow;

    if (!actor->hasShadow) {
        return;
    }
    shadow = actor->shadow;
    if (shadow == NULL) {
        return;
    }
    if (shadow->unk_5d == 0) {
        if (shadow != NULL) {
            Task_Destroy(shadow);
            actor->shadow = NULL;
        }
        actor->hasShadow = 0;
        return;
    }
    actor->shadow->x = func_ov002_0235867c(actor);
    actor->shadow->y = actor->unk_46 + func_ov002_02358600(actor);
}
