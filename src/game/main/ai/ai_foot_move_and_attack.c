#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_0209c948(void);
int func_0209f7ac(MapUnit *unit);
void func_020a9854(void);
void func_020ab014(void);
void func_020ad0d4(void);
void func_020ad704(void);
void func_020ad8f8(void);
void func_020ae37c(void);
void func_020ae9c0(void);
void func_020aeefc(void);
void func_020af5f8(void);
void func_020b0794(void);
void func_020b162c(void);
void func_020b2904(void);

/* Queue routine for infantry and mechs (func_0208f1d4 collects units
 * whose UnitStats.unk_20 is 1). func_020b162c picks direct or indirect
 * combat; the other steps are not named yet. */
void AiFoot_MoveAndAttack(void)
{
    func_0209c948();
    if (func_0209f7ac(GAME->actingUnit) >= 20) {
        func_020ae9c0();
    }
    if (GAME->players[GAME->currentPlayer].unk_2e != AI_BEHAVIOUR_DEFENSE) {
        func_020aeefc();
        func_020ad0d4();
    }
    if (GAME->actingUnit->hp <= 60) {
        func_020ad8f8();
    }
    func_020af5f8();
    func_020ae9c0();
    if (GAME->actingUnit->hp > 60) {
        func_020ad8f8();
    }
    func_020b162c();
    func_020ad704();
    if (GAME->actingUnit->hp <= 60) {
        func_020b0794();
    }
    func_020ae37c();
    func_020ab014();
    gAiTurnState[8] |= 0x20;
    func_020ae37c();
    func_020a9854();
    func_020b2904();
}
