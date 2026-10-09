#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Ai_ClearThreatMaps(void);
void func_020a9f64(void);
void AiUnit_SeekSeaTransport(void);
void func_020af540(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void func_020b297c(void);

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 10.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder10(void)
{
    Ai_ClearThreatMaps();
    func_020af540();
    /* rain also brings fog */
    if (GAME->unk_1c4->fog != 0 || GAME->unk_1c4->weather == WEATHER_RAIN) {
        AiUnit_PlanAttack();
    }
    func_020a9f64();
    AiUnit_SeekSeaTransport();
    func_020b297c();
    AiUnit_WaitInPlace();
}
