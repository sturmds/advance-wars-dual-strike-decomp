#include "types.h"
#include "game/Unk_02080440.h"
#include "game/strike_target.h"

s32 Rng_Range(s32 min, s32 max);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Picks the strike site with the best score for playerSlot and writes it
 * to site[0] (x) and site[1] (y). Returns 0 when no site scores. In
 * STRIKE_MODE_AI_FUNDS, site also gives the tile to skip on input (see
 * Strike_ScoreSite). damage 30 is 3 HP. */
int Strike_FindBestSite(s16* site, int playerSlot, int visibleOnly, int mode) {
  int skipX;
  int skipY;
  int best;
  int radius;
  int damage;
  int x;
  int y;
  int score;

  switch (mode) {
    case STRIKE_MODE_AI_FUNDS:
      skipX = site[0];
      skipY = site[1];
      gStrikeUnitScore = StrikeScore_FundsDamage;
      gStrikeSiteScore = StrikeSiteScore_NetFunds;
      radius = 2;
      damage = 30;
      best = 0;
      break;
    case STRIKE_MODE_FUNDS:
    case STRIKE_MODE_HP:
    case STRIKE_MODE_INFANTRY:
      skipY = -1;
      skipX = skipY;
      radius = 2;
      damage = 30;
      best = STRIKE_NO_SITE;
      switch (mode) {
        case STRIKE_MODE_FUNDS:
          gStrikeUnitScore = StrikeScore_FundsDamage;
          break;
        case STRIKE_MODE_HP:
          gStrikeUnitScore = StrikeScore_HpDamage;
          break;
        case STRIKE_MODE_INFANTRY:
          gStrikeUnitScore = StrikeScore_InfantryDamage;
          break;
      }
      gStrikeSiteScore = StrikeSiteScore_OwnTimes4;
      break;
    case STRIKE_MODE_RANDOM:
      skipY = -1;
      skipX = skipY;
      radius = 2;
      damage = 30;
      best = STRIKE_NO_SITE;
      switch (Rng_Range(0, 1)) {
        case 0:
          gStrikeUnitScore = StrikeScore_FundsDamage;
          break;
        case 1:
          gStrikeUnitScore = StrikeScore_HpDamage;
          break;
      }
      gStrikeSiteScore = StrikeSiteScore_OwnTimes2;
      break;
    case STRIKE_MODE_RANDOM_8HP:
      skipY = -1;
      skipX = skipY;
      radius = 2;
      damage = 80;
      best = STRIKE_NO_SITE;
      switch (Rng_Range(0, 1)) {
        case 0:
          gStrikeUnitScore = StrikeScore_FundsDamage;
          break;
        case 1:
          gStrikeUnitScore = StrikeScore_HpDamage;
          break;
      }
      gStrikeSiteScore = StrikeSiteScore_OwnTimes2;
      break;
    default:
      return 0;
  }

  for (y = 0; y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight;
       y++) {
    for (x = 0; x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth;
         x++) {
      score = Strike_ScoreSite(playerSlot, x, y, radius, damage, visibleOnly,
                               skipX, skipY);
      if (score > best) {
        site[0] = x;
        best = score;
        site[1] = y;
      }
    }
  }

  if (mode != STRIKE_MODE_AI_FUNDS) {
    if (best != STRIKE_NO_SITE) {
      return 1;
    }
  } else if (best != 0) {
    return 1;
  }
  return 0;
}
