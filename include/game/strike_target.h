#ifndef GAME_STRIKE_TARGET_H
#define GAME_STRIKE_TARGET_H

#include "types.h"
#include "game/map_unit.h"

/* Target choice for the CO power missile strikes. Strike_FindBestSite
 * scores every map tile with Strike_ScoreSite, which adds up
 * gStrikeUnitScore over the units in the blast diamond, separately for
 * own-team and enemy units, then combines the two sums with
 * gStrikeSiteScore. */

/* Strike_FindBestSite modes. Covering Fire uses 4, 3 and 5, one per
 * missile. */
#define STRIKE_MODE_AI_FUNDS 0   /* AI check: net funds damage, at least 9000 */
#define STRIKE_MODE_RANDOM 1     /* 3 HP; funds or HP score at random */
#define STRIKE_MODE_RANDOM_8HP 2 /* 8 HP; funds or HP score at random */
#define STRIKE_MODE_FUNDS 3
#define STRIKE_MODE_HP 4
#define STRIKE_MODE_INFANTRY 5

/* Score of one unit hit for damage (in hp units, 100 = full); isEnemy is
 * 1 for units on another team than the striking player. */
typedef int (*StrikeUnitScoreFunc)(MapUnit* unit, int damage, int isEnemy);

/* Site score from the own-team and enemy sums. */
typedef int (*StrikeSiteScoreFunc)(int ownScore, int enemyScore);

extern StrikeUnitScoreFunc gStrikeUnitScore;
extern StrikeSiteScoreFunc gStrikeSiteScore;

int StrikeScore_FundsDamage(MapUnit* unit, int damage, int isEnemy);
int StrikeScore_HpDamage(MapUnit* unit, int damage, int isEnemy);
int StrikeScore_InfantryDamage(MapUnit* unit, int damage, int isEnemy);

/* enemyScore - 2 * ownScore; 0x80000001 (no site) when enemyScore is 0. */
int StrikeSiteScore_OwnTimes2(int ownScore, int enemyScore);
/* enemyScore - 4 * ownScore; 0x80000001 (no site) when enemyScore is 0. */
int StrikeSiteScore_OwnTimes4(int ownScore, int enemyScore);
/* enemyScore - ownScore, or 0 when that is below 9000. */
int StrikeSiteScore_NetFunds(int ownScore, int enemyScore);

/* Site score below every real score: no site found. */
#define STRIKE_NO_SITE ((int)0x80000001)

int Strike_ScoreSite(int playerSlot, int x, int y, int radius, int damage,
                     int visibleOnly, int skipX, int skipY);

int Strike_FindBestSite(s16* site, int playerSlot, int visibleOnly, int mode);

#endif
