#include "types.h"
#include "game/battle_actor.h"

void Task_Destroy(void *task);
s16 BattleCellTask_GetOffsetY(BattleActor *actor);
int BattleCellTask_GetOffsetX(BattleActor *actor);

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
    actor->shadow->x = BattleCellTask_GetOffsetY(actor);
    actor->shadow->y = actor->unk_46 + BattleCellTask_GetOffsetX(actor);
}
