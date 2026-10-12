#ifndef GAME_AI_SITE_H
#define GAME_AI_SITE_H

#include "types.h"

/* AiSite.type, and the siteType argument of the AiSite_Pick* functions. */
#define SITE_BASE 1
#define SITE_AIRPORT 2
#define SITE_PORT 3
#define SITE_TYPE_ANY 4 /* argument only: any type */

/* AiSite.kind: the record is skipped. */
#define SITE_KIND_SKIPPED 0xfd

/* One 12-byte record of the AI's site list, data_0229e5bc (count in AiBuildState.recordCount). Only the
 * fields seen so far are listed. */
typedef struct AiSite {
  u8 x;
  u8 y;
  u8 type; /* SITE_BASE, SITE_AIRPORT or SITE_PORT: the factory kind */
  u8 kind; /* SITE_KIND_SKIPPED: the record is skipped; 0xfe: no action found (AiTurn_TrySiteAction); 0xff: rejected while AiTurn_PickUnitToBuild tries a unit type; AiSites_ClearRejectedMarks resets 0xff to 127, AiSites_ClearAllMarks both */
  s8 group; /* AiSiteGroup_RateSurroundings works on the records of one group; -1 marks a group's first record (AiSites_BuildGroups) */
  s8 unitBalance; /* AiSiteGroup_RateSurroundings: +1 per enemy foot unit near the site (+2 for others), -1 per own-team unit */
  u8 enemyCount;  /* AiSiteGroup_RateSurroundings: enemy units near the site */
  u8 enemyDistance; /* AiSiteGroup_RateSurroundings: distance to the nearest enemy unit that can reach the site */
  u8 propertyMoves; /* AiSiteGroup_RateSurroundings: Infantry move cost to the nearest property the team does not own; 0xff: not set */
  u8 unk_09; /* lowest range-map value of a current player's Infantry or Mech (func_02092134) */
  u8 unk_0a; /* same for a unit waiting for a T-Copter that fits a Lander */
  u8 score;  /* set by AiSite_ScorePiperunnerNeed */
} AiSite;

typedef char AiSite_size_check[sizeof(AiSite) == 12 ? 1 : -1];

extern AiSite data_0229e5bc[];

#endif
