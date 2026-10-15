#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

BOOL Player_IsInGame(u8 player);
void UpdatePlayerSkills(int player);
void Map_CountPlayerProperties(void);

/* Players 1 to 4; Player_IsInGame tells whether one is still in the game. */
#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* For each player in the game, picks the movement cost chart from the
 * front CO's Prairie Dog and Pathfinder skills (an index into
 * CoPowerBlock.movementCost), and sums the terrain star cuts of the
 * enemy COs' current power blocks. */
void MovementCost(void) {
  int p;
  int player; /* needed to match: declared before enemy */
  int enemy;

  for (p = 1; p < ARMY_SLOT_COUNT; p++) {
    if (Player_IsInGame(p)) {
      UpdatePlayerSkills(p);
    }
  }

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }

    PLAYERS[player].moveChart = MOVE_CHART_DEFAULT;
    if (CheckSkillsEquipped(PLAYERS[player].skills[0], SKILL_PRAIRIE_DOG)) {
      if (CheckSkillsEquipped(PLAYERS[player].skills[0], SKILL_PATHFINDER)) {
        PLAYERS[player].moveChart = MOVE_CHART_BOTH;
      } else {
        PLAYERS[player].moveChart = MOVE_CHART_PRAIRIE_DOG;
      }
    } else if (CheckSkillsEquipped(PLAYERS[player].skills[0],
                                   SKILL_PATHFINDER)) {
      PLAYERS[player].moveChart = MOVE_CHART_PATHFINDER;
    }

    /* Each enemy star cut takes 10 off this player's terrain defense value. */
    PLAYERS[player].terrainDefense = 0;
    for (enemy = 1; enemy < ARMY_SLOT_COUNT; enemy++) {
      if (Player_IsInGame(enemy) &&
          PLAYERS[player].team != PLAYERS[enemy].team) {
        /* Needed to match: the original reads the CO id with a byte
                 * load and the power state with shifts, as bit fields would. */
        PLAYERS[player].terrainDefense -=
            gCoTable[PLAYERS[enemy].front.id & PLAYER_CO_ID_MASK]
                .blocks[PLAYERS[enemy].front.powerState]
                .terrainStarCut *
            10;
      }
    }
  }

  Map_CountPlayerProperties();
}
