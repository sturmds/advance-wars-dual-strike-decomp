#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020a0bc0(void);
void func_020a0bdc(void);
void func_020a0bfc(void);

/* Queue routine for the direct units AiAction_QueueAllDirectUnitsFog
 * collects: runs one of three action sequences on the acting unit, chosen
 * by its AI order. The flag test is the inverse of the one in
 * AiUnit_ActByOrderGroup. */
void AiUnit_ActByOrderFog(void)
{
    if (data_027c027c->unk_48[data_027c0284]->actingUnit->unk_0a_12 == 2) {
        gAiTurnState->unk_08 |= 0x80;
    }

    switch (data_027c027c->unk_48[data_027c0284]->actingUnit->aiOrder) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
        func_020a0bfc();
        break;
    case 1:
    case 10:
        func_020a0bdc();
        break;
    case 7:
        func_020a0bc0();
        break;
    }
}
