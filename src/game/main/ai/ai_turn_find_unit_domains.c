#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

typedef struct DomainBits {
  u8 bits[4];
} DomainBits;

extern DomainBits
    data_02161a64; /* AI_DOMAIN_* bit for each gTerrainFactoryKind value */
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

BOOL Map_IsTileOwnedByTeamOf(int player, int x, int y);
BOOL Player_IsInGame(u8 player);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define TILE(x, y) (STATE->mapTiles->tiles[TILE_INDEX(x, y)])

/* Fills the AiTurnState domain fields (domainsAny: everyone, domainsOwn: the
 * current player, domainsEnemy: other teams). Factories (base, airport, port)
 * give ground, air and sea bits by tile ownership; then every unit of a
 * player in the game adds sea for ships and air for aircraft; ground
 * units add nothing. The names are guesses from what the bits record. */
void AiTurn_FindUnitDomains(void) {
  DomainBits domainBit = data_02161a64;
  int x;
  int y;
  int player;
  int slot;
  MapUnit* unit;

  gAiTurnState->domainsAny = 0;
  gAiTurnState->domainsOwn = 0;
  gAiTurnState->domainsEnemy = 0;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (!Map_IsTileOwnedByTeamOf(STATE->currentPlayer, x, y)) {
        gAiTurnState->domainsEnemy |=
            domainBit.bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(TILE(x, y))]];
      }
      if (STATE->unk_04 == (TILE(x, y) & 0xe0)) {
        gAiTurnState->domainsOwn |=
            domainBit.bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(TILE(x, y))]];
      }
      gAiTurnState->domainsAny |=
          domainBit.bits[gTerrainFactoryKind[MAP_TILE_TERRAIN(TILE(x, y))]];
    }
  }
  gAiTurnState->factoryDomainsOwn = gAiTurnState->domainsOwn;
  gAiTurnState->factoryDomainsEnemy = gAiTurnState->domainsEnemy;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA) {
        gAiTurnState->domainsAny |= AI_DOMAIN_SEA;
        if (Unit_IsCurrentPlayers(slot) == 1) {
          gAiTurnState->domainsOwn |= AI_DOMAIN_SEA;
        }
        if (STATE->players[STATE->currentPlayer].team !=
            STATE->players[player].team) {
          gAiTurnState->domainsEnemy |= AI_DOMAIN_SEA;
        }
      } else if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR) {
        gAiTurnState->domainsAny |= AI_DOMAIN_AIR;
        if (Unit_IsCurrentPlayers(slot) == 1) {
          gAiTurnState->domainsOwn |= AI_DOMAIN_AIR;
        }
        if (STATE->players[STATE->currentPlayer].team !=
            STATE->players[player].team) {
          gAiTurnState->domainsEnemy |= AI_DOMAIN_AIR;
        }
      }
    }
  }
}
