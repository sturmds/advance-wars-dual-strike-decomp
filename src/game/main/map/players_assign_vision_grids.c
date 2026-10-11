#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/vision.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_0229eaf0[];
extern u16 data_02139348[];

BOOL Player_IsInGame(u8 player);
void func_020c03d8(void);
BOOL func_020c5fd4(u16 player);
void Player_SetAlliesVisionGrids(u16 player, int gridMask);  /* sets unk_1b of player's allies */
BOOL func_020c6514(u16 player);                /* TRUE: an ally has data_0229eaf0 kind 1 */
int Player_GetSlotRelation(u16 player, u16 other);      /* SLOT_RELATION_* */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Player slots 1-4 counted on from a slot wrap round past 4. */
#define SLOT_WRAP(n) ((n) >= 5 ? (n) - 4 : (n))

/* Sets Player.unk_1b, the vision grids each player's sight goes into (see
 * game/vision.h), for the current player's turn. The current player gets the
 * bits data_02139348 holds for its data_0229eaf0 kind; when that is 1, it
 * also gets bit 1 if func_020c6514 finds an ally of kind 1. Its allies get the
 * same bits. When func_020c5fd4 returns TRUE, each enemy of kind 1 and that
 * enemy's allies get the other grid. If no player in the game has kind 1 or
 * bit 1, the current player's side gets bit 1 too. */
void Players_AssignVisionGrids(void)
{
    u16 i;
    u16 gridMask;
    int enemyAlly;
    u16 j;
    u16 enemy;

    /* needed to match: slot 0 is cleared before the loop */
    STATE->players[0].unk_1b = 0;
    for (i = 1; i < 5; i++) {
        STATE->players[i].unk_1b = 0;
    }
    func_020c03d8();
    gridMask = data_02139348[data_0229eaf0[STATE->currentPlayer]];
    if (gridMask == 1 && func_020c6514(STATE->currentPlayer)) {
        gridMask |= 2;
    }
    STATE->players[STATE->currentPlayer].unk_1b = gridMask;
    Player_SetAlliesVisionGrids(STATE->currentPlayer, gridMask);
    if (func_020c5fd4(STATE->currentPlayer)) {
        for (i = 1; i < 4; i++) {
            enemy = SLOT_WRAP(STATE->currentPlayer + i);
            if (Player_GetSlotRelation(STATE->currentPlayer, enemy) == SLOT_RELATION_ENEMY && data_0229eaf0[enemy] == 1) {
                STATE->players[enemy].unk_1b = gridMask ^ 3;
                for (j = 1; j < 4; j++) {
                    enemyAlly = enemy + j;
                    if (Player_GetSlotRelation(enemy, SLOT_WRAP(enemyAlly)) == SLOT_RELATION_ALLY) {
                        if (enemyAlly >= 5) {
                            enemyAlly -= 4;
                        }
                        STATE->players[enemyAlly].unk_1b = gridMask ^ 3;
                    }
                }
            }
        }
    }
    for (i = 1; i < 6; i++) {
        if (i > 4) {
            STATE->players[STATE->currentPlayer].unk_1b |= 2;
            Player_SetAlliesVisionGrids(STATE->currentPlayer, STATE->players[STATE->currentPlayer].unk_1b);
            break;
        }
        if (Player_IsInGame(i) && data_0229eaf0[i] == 1) {
            break;
        }
        if (Player_IsInGame(i) && (STATE->players[i].unk_1b & 2)) {
            break;
        }
    }
    STATE->unk_20 = 0;
    STATE->unk_22 = 1;
}
