#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"
#include "game/unit_stats.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Player_AddKill(int killerSlot, int victimSlot,
                    int victimType); /* counts a kill for killerSlot */
void Unit_CountDestroyed(
    MapUnit* unit); /* counts the unit and its cargo as destroyed */
void Combat_RecordDamage(Combatant* combatant,
                         Combatant* other); /* damage taken and funds damage */
void Combatant_WriteBack(
    Combatant* combatant,
    Combatant*
        other); /* writes hp and ammo back to the unit; at 0 hp calls func_020edb84 */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define COMBATANT_OWNER(side) \
  MAP_UNIT_OWNER(STATE, STATE->combatants[side].unit)

/* Applies the battle worked out in combatants[0] (attacker) and [1]
 * (defender) to the map. A side at 0 hp counts as a kill for the other
 * side's player; a human player also gets the destroyed unit counted, and
 * destroying an Oozium counts toward the Oozium total. Then both sides'
 * hp and ammo go back to their units. */
void Combat_ApplyResult(void) {
  if (STATE->combatants[0].hp <= 0) {
    Player_AddKill(COMBATANT_OWNER(1), COMBATANT_OWNER(0),
                   STATE->combatants[0].unit->type);
    if (STATE->players[COMBATANT_OWNER(1)].controller ==
        PLAYER_CONTROLLER_HUMAN) {
      Unit_CountDestroyed(STATE->combatants[0].unit);
    }
    if (STATE->combatants[0].unit->type == UNIT_OOZIUM) {
      if (STATE->players[COMBATANT_OWNER(1)]
                  .destroyedCounts[PLAYER_DESTROYED_OOZIUM] +
              1 >=
          99) {
        STATE->players[COMBATANT_OWNER(1)]
            .destroyedCounts[PLAYER_DESTROYED_OOZIUM] = 99;
      } else {
        STATE->players[COMBATANT_OWNER(1)]
            .destroyedCounts[PLAYER_DESTROYED_OOZIUM]++;
      }
    }
  }
  if (STATE->combatants[1].hp <= 0) {
    Player_AddKill(COMBATANT_OWNER(0), COMBATANT_OWNER(1),
                   STATE->combatants[1].unit->type);
    if (STATE->players[COMBATANT_OWNER(0)].controller ==
        PLAYER_CONTROLLER_HUMAN) {
      Unit_CountDestroyed(STATE->combatants[1].unit);
    }
    if (STATE->combatants[1].unit->type == UNIT_OOZIUM) {
      if (STATE->players[COMBATANT_OWNER(0)]
                  .destroyedCounts[PLAYER_DESTROYED_OOZIUM] +
              1 >=
          99) {
        STATE->players[COMBATANT_OWNER(0)]
            .destroyedCounts[PLAYER_DESTROYED_OOZIUM] = 99;
      } else {
        STATE->players[COMBATANT_OWNER(0)]
            .destroyedCounts[PLAYER_DESTROYED_OOZIUM]++;
      }
    }
  }
  Combat_RecordDamage(&STATE->combatants[0], &STATE->combatants[1]);
  Combat_RecordDamage(&STATE->combatants[1], &STATE->combatants[0]);
  Combatant_WriteBack(&STATE->combatants[0], &STATE->combatants[1]);
  Combatant_WriteBack(&STATE->combatants[1], &STATE->combatants[0]);
}
