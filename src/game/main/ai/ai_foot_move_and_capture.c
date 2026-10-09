#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_0209c948(void);
int func_0209f7ac(MapUnit *unit);
void func_020ad0d4(void);
void func_020ad8f8(void);
void func_020ae9c0(void);
void func_020aeefc(void);
void func_020af5f8(void);

/* Queue routine for infantry and mechs set by func_0208f308. Runs the
 * same first steps as AiFoot_MoveAndAttack and stops before the attack
 * dispatcher. func_0209f7ac calls CaptureIncrease. */
void AiFoot_MoveAndCapture(void)
{
    func_0209c948();
    if (func_0209f7ac(GAME->activeUnit) >= 20) {
        func_020ae9c0();
    }
    if (GAME->players[GAME->currentPlayer].unk_2e != AI_BEHAVIOUR_DEFENSE) {
        func_020aeefc();
        func_020ad0d4();
    }
    if (GAME->activeUnit->hp <= 60) {
        func_020ad8f8();
    }
    func_020af5f8();
    func_020ae9c0();
    if (GAME->activeUnit->hp > 60) {
        func_020ad8f8();
    }
}
