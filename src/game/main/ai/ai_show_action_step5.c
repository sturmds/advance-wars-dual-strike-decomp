#include "types.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"

void func_020deac4(void);
void func_020dd9b4(int x, int y, int target, int arg, int mode);

void AiShowAction_Step5(void)
{
    func_020deac4();
    func_020dd9b4(gUnitAction.x, gUnitAction.y, gUnitAction.target, gUnitAction.unk_07, 3);
    gAiTurnState->actionStep = 6;
}
