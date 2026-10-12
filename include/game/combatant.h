#ifndef GAME_COMBATANT_H
#define GAME_COMBATANT_H

#include "types.h"
#include "game/map_unit.h"

/* Combatant.weapon */
#define COMBAT_WEAPON_NONE 0
#define COMBAT_WEAPON_PRIMARY 1
#define COMBAT_WEAPON_SECONDARY 5

/* One side of a battle being worked out, 0x18 bytes. Two of them sit at
 * Unk_02080440_48.combatants: attacker, then defender. */
typedef struct Combatant {
  MapUnit* unit;
  s16 terrain;        /* terrain id under the unit */
  s16 terrainDefense; /* percent, 10 per star, from DoubleTerrainStars */
  s16 hp;             /* unit->hp, then hp left after the battle */
  s16 ammo;           /* copy of unit->ammo */
  s16 damage;
  s16 defense;      /* percent; capped at 200 */
  s16 baseDamage;   /* from the damage chart */
  s16 hpLost;       /* hp the other side's hit takes, see Combatant_TakeHit */
  s16 damageNoLuck; /* damage before the luck rolls */
  s16 weapon;       /* COMBAT_WEAPON_*, set by Combatant_PickWeapon */
} Combatant;

typedef char Combatant_size_check[sizeof(Combatant) == 0x18 ? 1 : -1];

#endif
