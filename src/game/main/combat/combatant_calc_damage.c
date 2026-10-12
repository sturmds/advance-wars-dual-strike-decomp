#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Rng_Next(void);                          /* random number */
s32 HwMod32(s32 numerator, s32 denominator); /* remainder */
int Player_GetGoodLuck(int playerIndex);
int Player_GetBadLuck(int playerIndex);
int Combatant_GetCounterBonus(Combatant* combatant);

/* Finishes one side's numbers once baseDamage is set: adds terrain defense
 * scaled by the unit's shown hp (1-10) to defense, capped at 200; scales
 * baseDamage by the counter bonus when isCounter is 1; then adds a good luck
 * roll (0 to goodLuck - 1) and takes away a bad luck roll. Without rollLuck
 * the rolls only happen while luckOnRequestOnly is clear. damageNoLuck keeps the damage
 * before the rolls. */
void Combatant_CalcDamage(Combatant* combatant, int isCounter, int rollLuck) {
  int percent = 100;
  int displayHp;
  int goodLuck;
  int badLuck;
  Unk_02080440_48* state;

  if (combatant->unit->hp == 0) {
    displayHp = 0;
  } else {
    displayHp = HwDiv32(combatant->unit->hp - 1, 10) + 1;
  }
  combatant->defense += HwDiv32(combatant->terrainDefense * displayHp, 10);
  if (combatant->defense >= 200) {
    combatant->defense = 200;
  }
  combatant->damage = combatant->baseDamage;
  if (isCounter == 1) {
    percent += Combatant_GetCounterBonus(combatant);
  }
  combatant->damage = HwDiv32(combatant->damage * percent, 100);
  combatant->damageNoLuck = combatant->damage;
  if (combatant->damage == 0) {
    return;
  }
  state = data_027c027c->unk_48[data_027c0284];
  /* needed to match: | instead of || */
  if (!(rollLuck | (state->luckOnRequestOnly == 0))) {
    return;
  }
  goodLuck = HwMod32(
      Rng_Next(), Player_GetGoodLuck(MAP_UNIT_OWNER(state, combatant->unit)));
  badLuck = Player_GetBadLuck(
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], combatant->unit));
  if (badLuck != 0) {
    badLuck = HwMod32(Rng_Next(), badLuck);
  }
  combatant->damage = combatant->damage + goodLuck - badLuck;
  if (combatant->damage < 0) {
    combatant->damage = 0;
  }
}
