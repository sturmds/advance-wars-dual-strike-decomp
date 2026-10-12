#include "types.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

#define UNLIMITED_MOVE 110

BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int GetUnitMaxRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void AiSites_ClearAllMarks(void);
void AiTurn_QueueBuildAtSite(int index);
int AiSite_PickNearestFoot(int arg);
int AiSites_HasType(int arg);

/* Run first by AiTurn_TryBuildTCopter. Unless AiSites_HasType(3) returns 1,
 * takes each site that AiSite_PickNearestFoot(2) returns in turn (-1: none left) and
 * fills the range map with Mech moves from it over the whole map. A site
 * is skipped (kind 0xfe) when a unit in reach stands on a tile that
 * Map_IsCapturableByActingUnit accepts, when an enemy vehicle in reach has
 * a weapon, or when the player's foot units in reach number fewer than
 * three per empty T-Copter in reach, the new one included. The first site
 * that passes gets a T-Copter. AiSites_ClearAllMarks then resets the skipped kinds.
 * Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildTCopterForFoot(void) {
  int slot;
  int index;
  MapUnit* unit;
  u8 copters;
  u8 foot;
  int x;
  AiSite* site;
  int y;

  if (AiSites_HasType(3) == 1) {
    return 0;
  }
  for (;;) {
    index = AiSite_PickNearestFoot(2);
    if (index == -1) {
      AiSites_ClearAllMarks();
      return 0;
    }
    site = &data_0229e5bc[index];
    RangeMap_FillMoves(site->x, site->y, UNIT_MECH, UNLIMITED_MOVE, 0);
    foot = 0;
    copters = 0;
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
        if (slot == 0) {
          continue;
        }
        if (Map_IsCapturableByActingUnit(x, y) == 1) {
          goto skip;
        }
        unit = &STATE->units[slot];
        if (STATE->players[STATE->currentPlayer].team !=
            STATE->players[(slot >> 6) + 1].team) {
          if (unit->type > UNIT_MECH &&
              GetUnitMaxRange(STATE->currentPlayer, unit->type) != 0) {
            goto skip;
          }
        } else if (Unit_IsCurrentPlayers(slot) == 1) {
          if (unit->type == UNIT_T_COPTER && unit->cargo[0] == 0) {
            copters++;
          }
          if (unit->type <= UNIT_MECH) {
            foot++;
          }
        }
      }
    }
    if ((copters + 1) * 3 > foot) {
      goto skip;
    }
    gUnitAction.target = UNIT_T_COPTER;
    AiTurn_QueueBuildAtSite(index);
    AiSites_ClearAllMarks();
    return 1;
  skip:
    site->kind = 0xfe;
  }
}
