#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7a9c.h"
#include "game/Unk_02290118.h"
#include "game/play_records.h"

int Player_GetCurrentOrFirstInGame(void);
int BattleScore_Speed(int player);
int BattleScore_Power(int player);
int BattleScore_Technique(int player);
int BattleScore_Total(int player); /* speed + power + technique */
int Score_ToRank_020e5428(int score);  /* rank 2-5 for a score */
BOOL Player1Team_HasPlayerInGame(void);
int Campaign_IsHardMode(void);
int ProfileHeaders_UpdateBest(int profile, int value);
void WinLoseSides_Fill(void);
void BattleResults_Fill(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* End-of-battle score into data_022a7a9c: speed, power and technique of
 * the current player (player), their sum (score)
 * and its rank; Survival halves all but the rank. Then the
 * Player.destroyedCounts counts of the human players summed into unk_08-unk_0c,
 * unk_0d set on maps 242 and 247, and totalPoints: the sum plus
 * the bonus points, 0 when player 1's team is out or in Versus, doubled on
 * campaign hard mode. In the campaign, previousBest gets the profile's earlier
 * best from ProfileHeaders_UpdateBest. Last, runs WinLoseSides_Fill and
 * BattleResults_Fill. */
void BattleScore_Compute(void)
{
    int i;
    int total;
    int oozium;
    int best;

    data_022a7a9c.player = Player_GetCurrentOrFirstInGame();
    data_022a7a9c.speed = BattleScore_Speed(data_022a7a9c.player);
    data_022a7a9c.power = BattleScore_Power(data_022a7a9c.player);
    data_022a7a9c.technique = BattleScore_Technique(data_022a7a9c.player);
    data_022a7a9c.score = BattleScore_Total(data_022a7a9c.player);
    data_022a7a9c.rank = Score_ToRank_020e5428(data_022a7a9c.score);
    if (STATE->gameMode == GAME_MODE_SURVIVAL) {
        data_022a7a9c.speed >>= 1;
        data_022a7a9c.power >>= 1;
        data_022a7a9c.technique >>= 1;
        data_022a7a9c.score >>= 1;
    }

    data_022a7a9c.unk_08 = 0;
    data_022a7a9c.unk_09 = 0;
    data_022a7a9c.unk_0a = 0;
    data_022a7a9c.unk_0b = 0;
    data_022a7a9c.unk_0c = 0;
    data_022a7a9c.unk_0d = 0;
    for (i = 1; i < 5; i++) {
        if (STATE->players[i].controller == PLAYER_CONTROLLER_HUMAN) {
            data_022a7a9c.unk_08 += STATE->players[i].destroyedCounts[0];
            data_022a7a9c.unk_09 += STATE->players[i].destroyedCounts[1];
            data_022a7a9c.unk_0a += STATE->players[i].destroyedCounts[4];
            data_022a7a9c.unk_0b += STATE->players[i].destroyedCounts[2];
            data_022a7a9c.unk_0c += STATE->players[i].destroyedCounts[PLAYER_DESTROYED_OOZIUM];
            if (STATE->unk_1c4->mapId == 242 || STATE->unk_1c4->mapId == 247) {
                data_022a7a9c.unk_0d = 1;
            } else {
                data_022a7a9c.unk_0d = 0;
            }
        }
    }

    /* bonus points per count; Oozium destroyed give at most 999 */
    total = data_022a7a9c.score;
    total += data_022a7a9c.unk_08 * 30;
    total += data_022a7a9c.unk_09 * 10;
    total += data_022a7a9c.unk_0a * 20;
    total += data_022a7a9c.unk_0b * 5;
    oozium = data_022a7a9c.unk_0c * 40;
    if (oozium >= 999) {
        oozium = 999;
    }
    total += oozium;
    total += data_022a7a9c.unk_0d * 30;
    if (!Player1Team_HasPlayerInGame()) {
        total = 0;
    }
    switch (STATE->gameMode) {
    case GAME_MODE_CAMPAIGN:
        if (Campaign_IsHardMode()) {
            total *= 2;
        }
        break;
    case GAME_MODE_VERSUS:
        total = 0;
        break;
    }
    if (STATE->gameMode != GAME_MODE_CAMPAIGN) {
        best = 0;
    } else {
        best = ProfileHeaders_UpdateBest(data_02290118.unk_00, total);
    }
    data_022a7a9c.previousBest = best;
    data_022a7a9c.totalPoints = total;
    data_022a7a9c.unk_10 = gPlayRecords.points - best;
    WinLoseSides_Fill();
    BattleResults_Fill();
}
