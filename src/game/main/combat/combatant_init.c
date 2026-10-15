#include "types.h"
#include "game/Unk_02080440.h"
#include "game/combatant.h"

int DoubleTerrainStars(int playerIndex, int unitType, int terrain);

/* Fills one side of a battle from a unit slot: terrain under the unit, its
 * terrain defense, hp and ammo. Clears the damage chart results. */
void Combatant_Init(Combatant* combatant, s16 unitIndex) {
  MapTiles* map;

  combatant->unit = &data_027c027c->unk_48[data_027c0284]->units[unitIndex];
  map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  combatant->terrain = MAP_TILE_TERRAIN(
      map->tiles[map->rowStart[combatant->unit->y] + combatant->unit->x]);
  combatant->terrainDefense = DoubleTerrainStars(
      MAP_UNIT_OWNER(data_027c027c->unk_48[data_027c0284], combatant->unit),
      combatant->unit->type, combatant->terrain);
  combatant->hp = combatant->unit->hp;
  combatant->ammo = combatant->unit->ammo;
  combatant->weapon = COMBAT_WEAPON_NONE;
  combatant->baseDamage = 0;
  combatant->hpLost = 0;
}
