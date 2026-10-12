#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"

#define DAMAGE_STAT_MAX 999999999

int Unit_GetValuePerHp(int playerIndex, int unitType);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern PlayRecords gPlayRecords;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds the funds value of hpLost hp of unit (hp times its value per hp) to
 * its owner's damage taken, and for a human owner outside game mode 4 also
 * to humanDamageTaken. Both totals stop at 999999999. */
void Unit_AddDamageTaken(MapUnit* unit, int hpLost) {
  int owner = MAP_UNIT_OWNER(STATE, unit);
  u32 value = hpLost * Unit_GetValuePerHp(owner, unit->type);
  u32 total;

  STATE->players[owner].damageTaken += value;
  if (STATE->players[owner].damageTaken > DAMAGE_STAT_MAX) {
    STATE->players[owner].damageTaken = DAMAGE_STAT_MAX;
  }
  if (STATE->players[owner].controller != PLAYER_CONTROLLER_HUMAN) {
    return;
  }
  if (STATE->gameMode == 4) {
    return;
  }
  total = gPlayRecords.humanDamageTaken + value;
  if (total < DAMAGE_STAT_MAX) {
    gPlayRecords.humanDamageTaken = total;
  } else {
    gPlayRecords.humanDamageTaken = DAMAGE_STAT_MAX;
  }
}
