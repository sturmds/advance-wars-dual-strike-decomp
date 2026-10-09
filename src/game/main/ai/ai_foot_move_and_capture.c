#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_0209c948(void);
int func_0209f7ac(MapUnit *unit);
void AiFoot_PickAttackTarget(void);
void AiFoot_JoinCapturingUnit(void);
void AiFoot_PickCaptureTarget(void);
void AiFoot_AttackCapturingUnit(void);
void func_020af5f8(void);

/* Queue routine for infantry and mechs set by AiAction_QueueFootUnits. Runs the
 * same first steps as AiFoot_MoveAndAttack and stops before the attack
 * dispatcher. func_0209f7ac calls CaptureIncrease. */
void AiFoot_MoveAndCapture(void)
{
    func_0209c948();
    if (func_0209f7ac(GAME->actingUnit) >= 20) {
        AiFoot_PickCaptureTarget();
    }
    if (GAME->players[GAME->currentPlayer].aiBehaviour != AI_BEHAVIOUR_DEFENSE) {
        AiFoot_AttackCapturingUnit();
        AiFoot_PickAttackTarget();
    }
    if (GAME->actingUnit->hp <= 60) {
        AiFoot_JoinCapturingUnit();
    }
    func_020af5f8();
    AiFoot_PickCaptureTarget();
    if (GAME->actingUnit->hp > 60) {
        AiFoot_JoinCapturingUnit();
    }
}
