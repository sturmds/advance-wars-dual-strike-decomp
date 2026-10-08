#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/skills.h"

BOOL func_020c237c(u8 player);
void UpdatePlayerSkills(int player);
void func_020c0aa8(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Players 1 to 4; func_020c237c tells whether one is still in the game. */
#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* For each player in the game, picks the movement cost chart from the
 * front CO's Prairie Dog and Pathfinder skills (an index into
 * CoPowerBlock.movementCost), and sums the terrain star cuts of the
 * enemy COs' current power blocks. */
void MovementCost(void)
{
    int p;
    int player; /* needed to match: declared before enemy */
    int enemy;

    for (p = 1; p < 5; p++) {
        if (func_020c237c(p)) {
            UpdatePlayerSkills(p);
        }
    }

    for (player = 1; player < 5; player++) {
        if (!func_020c237c(player)) {
            continue;
        }

        PLAYERS[player].moveChart = MOVE_CHART_DEFAULT;
        if (CheckSkillsEquipped(PLAYERS[player].skills[0], SKILL_PRAIRIE_DOG)) {
            if (CheckSkillsEquipped(PLAYERS[player].skills[0], SKILL_PATHFINDER)) {
                PLAYERS[player].moveChart = MOVE_CHART_BOTH;
            } else {
                PLAYERS[player].moveChart = MOVE_CHART_PRAIRIE_DOG;
            }
        } else if (CheckSkillsEquipped(PLAYERS[player].skills[0], SKILL_PATHFINDER)) {
            PLAYERS[player].moveChart = MOVE_CHART_PATHFINDER;
        }

        /* Each enemy star cut takes 10 off this player's terrain defense value. */
        PLAYERS[player].terrainDefense = 0;
        for (enemy = 1; enemy < 5; enemy++) {
            if (func_020c237c(enemy) && PLAYERS[player].team != PLAYERS[enemy].team) {
                /* Needed to match: the original reads the CO id with a byte
                 * load and the power state with shifts, as bit fields would. */
                PLAYERS[player].terrainDefense -=
                    gCoTable[*(u8 *)&PLAYERS[enemy].frontCo & PLAYER_CO_ID_MASK]
                        .blocks[(PLAYERS[enemy].frontCo << 20) >> 30]
                        .terrainStarCut * 10;
            }
        }
    }

    func_020c0aa8();
}
