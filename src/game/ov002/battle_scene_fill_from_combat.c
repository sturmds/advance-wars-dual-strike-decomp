#include "types.h"
#include "game/battle_scene.h"
#include "game/combatant.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

u8 Player_GetArmyStyle(int playerIndex);

#define GAME (data_027c027c->unk_48[data_027c0284])

#define TILE_OWNER(unit) \
  MAP_TILE_OWNER(        \
      GAME->mapTiles->tiles[(unit)->x + GAME->mapTiles->rowStart[(unit)->y]])

/* Fills Ov002_gBattleScene for the battle animation from the two units and
 * their Combatants: side 0 is unitA. A side counters when it has a weapon. */
void BattleScene_FillFromCombat(MapUnit* unitA, MapUnit* unitB,
                                Combatant* combatantA, Combatant* combatantB) {
  if (unitA == GAME->actingUnit) {
    Ov002_gBattleScene->attackerSide = 0;
  } else {
    Ov002_gBattleScene->attackerSide = 1;
  }
  Ov002_gBattleScene->weather = GAME->unk_1c4->weather;
  Ov002_gBattleScene->tileset = GAME->unk_1c4->tileset & 0x7f;
  BATTLE_SIDE_UNIT_TYPE(0) = unitA->type;
  BATTLE_SIDE_UNIT_TYPE(1) = unitB->type;

  if (combatantA->weapon == COMBAT_WEAPON_SECONDARY) {
    Ov002_gBattleScene->sides[0].usesSecondary = 1;
    Ov002_gBattleScene->sides[0].counters = 1;
  } else if (combatantA->weapon == COMBAT_WEAPON_NONE) {
    Ov002_gBattleScene->sides[0].usesSecondary = 0;
    Ov002_gBattleScene->sides[0].counters = 0;
  } else {
    Ov002_gBattleScene->sides[0].usesSecondary = 0;
    Ov002_gBattleScene->sides[0].counters = 1;
  }
  if (combatantB->weapon == COMBAT_WEAPON_SECONDARY) {
    Ov002_gBattleScene->sides[1].usesSecondary = 1;
    Ov002_gBattleScene->sides[1].counters = 1;
  } else if (combatantB->weapon == COMBAT_WEAPON_NONE) {
    Ov002_gBattleScene->sides[1].usesSecondary = 0;
    Ov002_gBattleScene->sides[1].counters = 0;
  } else {
    Ov002_gBattleScene->sides[1].usesSecondary = 0;
    Ov002_gBattleScene->sides[1].counters = 1;
  }

  /* needed to match: the 0/1 value is compared with 1 */
  if (((unitA->flags & MAP_UNIT_DIVED) ? 1 : 0) == 1) {
    Ov002_gBattleScene->sides[0].dived = 1;
  } else {
    Ov002_gBattleScene->sides[0].dived = 0;
  }
  if (((unitB->flags & MAP_UNIT_DIVED) ? 1 : 0) == 1) {
    Ov002_gBattleScene->sides[1].dived = 1;
  } else {
    Ov002_gBattleScene->sides[1].dived = 0;
  }

  Ov002_gBattleScene->sides[0].hpBefore = unitA->hp;
  Ov002_gBattleScene->sides[1].hpBefore = unitB->hp;
  Ov002_gBattleScene->sides[0].hpAfter = combatantA->hp;
  Ov002_gBattleScene->sides[1].hpAfter = combatantB->hp;
  Ov002_gBattleScene->sides[0].terrain = combatantA->terrain;
  Ov002_gBattleScene->sides[1].terrain = combatantB->terrain;
  Ov002_gBattleScene->sides[0].terrainDefense = combatantA->terrainDefense;
  Ov002_gBattleScene->sides[1].terrainDefense = combatantB->terrainDefense;
  Ov002_gBattleScene->sides[0].tileOwner = TILE_OWNER(unitA);
  Ov002_gBattleScene->sides[1].tileOwner = TILE_OWNER(unitB);
  Ov002_gBattleScene->sides[0].coId =
      GAME->players[MAP_UNIT_OWNER(GAME, unitA)].front.id;
  Ov002_gBattleScene->sides[1].coId =
      GAME->players[MAP_UNIT_OWNER(GAME, unitB)].front.id;
  Ov002_gBattleScene->sides[0].armyStyle =
      Player_GetArmyStyle(MAP_UNIT_OWNER(GAME, unitA));
  Ov002_gBattleScene->sides[1].armyStyle =
      Player_GetArmyStyle(MAP_UNIT_OWNER(GAME, unitB));
  Ov002_gBattleScene->sides[0].army =
      GAME->players[MAP_UNIT_OWNER(GAME, unitA)].armyColour;
  Ov002_gBattleScene->sides[1].army =
      GAME->players[MAP_UNIT_OWNER(GAME, unitB)].armyColour;
}
