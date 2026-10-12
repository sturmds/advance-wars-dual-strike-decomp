#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int abs(int value);
void Combatant_PickWeapon(Combatant* attacker, Combatant* defender,
                          s16 distance,
                          int isAttack); /* sets defense and baseDamage */
void Combat_StrikeAndCounter(
    Combatant* first,
    Combatant* second); /* first hits, then second hits back */
void Combat_ClampHp(
    Combatant* attacker,
    Combatant*
        defender); /* no hp below 0; one side survives at 1 if both would die */
void Combatant_Init(Combatant* combatant, s16 unitIndex);
void Combatant_CalcDamage(Combatant* combatant, int isCounter, int rollLuck);
BOOL CounterFirst(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Works out a battle between two unit slots with the attacker moved to pos
 * (x, y), luck included, and leaves the results in combatants[0] (attacker)
 * and combatants[1] (defender). The defender strikes first when its CO
 * counters first. The attacker's position is put back after. */
void Combat_SimulateBattle(s16 attackerIndex, s16 defenderIndex, s16* pos) {
  int x;
  int y;
  int distance;

  x = STATE->units[attackerIndex].x;
  STATE->units[attackerIndex].x = pos[0];
  y = STATE->units[attackerIndex].y;
  STATE->units[attackerIndex].y = pos[1];
  Combatant_Init(&STATE->combatants[0], attackerIndex);
  Combatant_Init(&STATE->combatants[1], defenderIndex);
  distance = abs(STATE->combatants[0].unit->x - STATE->combatants[1].unit->x) +
             abs(STATE->combatants[0].unit->y - STATE->combatants[1].unit->y);
  Combatant_PickWeapon(&STATE->combatants[0], &STATE->combatants[1], distance,
                       1);
  Combatant_PickWeapon(&STATE->combatants[1], &STATE->combatants[0], distance,
                       0);
  Combatant_CalcDamage(&STATE->combatants[0], 0, 1);
  Combatant_CalcDamage(&STATE->combatants[1], 1, 1);
  /* needed to match: the u8 cast */
  if (CounterFirst((u8)MAP_UNIT_OWNER(STATE, STATE->combatants[1].unit)) == 1) {
    Combat_StrikeAndCounter(&STATE->combatants[1], &STATE->combatants[0]);
  } else {
    Combat_StrikeAndCounter(&STATE->combatants[0], &STATE->combatants[1]);
  }
  Combat_ClampHp(&STATE->combatants[0], &STATE->combatants[1]);
  STATE->units[attackerIndex].x = x;
  STATE->units[attackerIndex].y = y;
}
