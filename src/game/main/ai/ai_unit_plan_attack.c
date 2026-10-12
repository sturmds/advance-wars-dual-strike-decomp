#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* 0: direct (max range 1), 1: indirect, 2: no weapon, 3: Black Bomb, 4: Oozium */
int UnitType_GetAttackKind(int unitType);
void AiUnit_PickDirectAttack(void);
void Ai_PlanIndirectAttack(void);

/* Queues the acting unit's attack, if it has one: direct units pick a
 * target next to them, indirect units one in range. */
void AiUnit_PlanAttack(void) {
  switch (UnitType_GetAttackKind(
      data_027c027c->unk_48[data_027c0284]->actingUnit->type)) {
    case 0:
      AiUnit_PickDirectAttack();
      break;
    case 1:
      Ai_PlanIndirectAttack();
      break;
  }
}
