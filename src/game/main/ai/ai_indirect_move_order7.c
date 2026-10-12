#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
void AiUnit_PlanAttack(void);

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 7.
 * AiUnit_PlanAttack only runs in Fog of War or rain, then the unit waits
 * where it is (action 2). */
void AiIndirect_MoveOrder7(void) {
  /* rain also brings fog */
  if (GAME->unk_1c4->fog != 0 || GAME->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_PerformAction(GAME->actingUnit->x, GAME->actingUnit->y, 2, 0, 0);
}
