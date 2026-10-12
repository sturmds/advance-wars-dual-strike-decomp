#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void Ai_ClearThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_MoveToEnemyProperty(void);
void func_020aac28(void);
void AiUnit_MoveToEnemyTarget(void);
void AiUnit_SeekSeaTransport(void);
void AiUnit_SeekSupply(void);
void AiUnit_HeadForObjectTarget(void);

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 1.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder1(void) {
  Ai_ClearThreatMaps();
  AiUnit_SeekSupply();
  /* rain also brings fog */
  if (GAME->unk_1c4->fog != 0 || GAME->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  func_020aac28();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
