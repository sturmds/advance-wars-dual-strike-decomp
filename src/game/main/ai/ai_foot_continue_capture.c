#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_0209c948(void);
int func_0209f7ac(MapUnit *unit);
void AiFoot_PickCaptureTarget(void);
void func_020aedec(void);
void AiFoot_AttackCapturingUnit(void);

/* Queue routine for infantry and mechs partway through a capture, set by
 * AiAction_QueueCapturingUnits. Starts like AiFoot_MoveAndCapture, then
 * runs func_020aedec. */
void AiFoot_ContinueCapture(void)
{
    func_0209c948();
    if (func_0209f7ac(GAME->actingUnit) >= 20) {
        AiFoot_PickCaptureTarget();
    }
    if (GAME->players[GAME->currentPlayer].aiBehaviour != AI_BEHAVIOUR_DEFENSE) {
        AiFoot_AttackCapturingUnit();
    }
    func_020aedec();
}
