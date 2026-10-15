#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

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

/* Works out a battle between two unit slots with the attacker moved to pos
 * (x, y), luck included, and leaves the results in combatants[0] (attacker)
 * and combatants[1] (defender). The defender strikes first when its CO
 * counters first. The attacker's position is put back after. */
void Combat_SimulateBattle(s16 attackerIndex, s16 defenderIndex, s16* pos) {
  int x;
  int y;
  int distance;

  x = MAP_STATE->units[attackerIndex].x;
  MAP_STATE->units[attackerIndex].x = pos[0];
  y = MAP_STATE->units[attackerIndex].y;
  MAP_STATE->units[attackerIndex].y = pos[1];
  Combatant_Init(&MAP_STATE->combatants[0], attackerIndex);
  Combatant_Init(&MAP_STATE->combatants[1], defenderIndex);
  distance =
      abs(MAP_STATE->combatants[0].unit->x - MAP_STATE->combatants[1].unit->x) +
      abs(MAP_STATE->combatants[0].unit->y - MAP_STATE->combatants[1].unit->y);
  Combatant_PickWeapon(&MAP_STATE->combatants[0], &MAP_STATE->combatants[1],
                       distance, 1);
  Combatant_PickWeapon(&MAP_STATE->combatants[1], &MAP_STATE->combatants[0],
                       distance, 0);
  Combatant_CalcDamage(&MAP_STATE->combatants[0], 0, 1);
  Combatant_CalcDamage(&MAP_STATE->combatants[1], 1, 1);
  /* needed to match: the u8 cast */
  if (CounterFirst(
          (u8)MAP_UNIT_OWNER(MAP_STATE, MAP_STATE->combatants[1].unit)) == 1) {
    Combat_StrikeAndCounter(&MAP_STATE->combatants[1],
                            &MAP_STATE->combatants[0]);
  } else {
    Combat_StrikeAndCounter(&MAP_STATE->combatants[0],
                            &MAP_STATE->combatants[1]);
  }
  Combat_ClampHp(&MAP_STATE->combatants[0], &MAP_STATE->combatants[1]);
  MAP_STATE->units[attackerIndex].x = x;
  MAP_STATE->units[attackerIndex].y = y;
}
