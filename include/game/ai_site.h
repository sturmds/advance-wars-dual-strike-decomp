#ifndef GAME_AI_SITE_H
#define GAME_AI_SITE_H

#include "types.h"

/* One 12-byte record of the AI's site list, data_0229e5bc (count in AiBuildState.recordCount). Only the
 * fields seen so far are listed. */
typedef struct AiSite {
    u8 x;
    u8 y;
    u8 active;                         /* 1: the record is in use */
    u8 kind;                           /* 0xfd: the record is skipped; 0xfe: no action found (AiTurn_TrySiteAction); 254 and up reset to 127 by func_0209378c */
    s8 group;                          /* AiSiteGroup_RateSurroundings works on the records of one group; -1 marks a group's first record (AiSites_BuildGroups) */
    s8 unitBalance;                    /* AiSiteGroup_RateSurroundings: +1 per enemy foot unit near the site (+2 for others), -1 per own-team unit */
    u8 enemyCount;                     /* AiSiteGroup_RateSurroundings: enemy units near the site */
    u8 enemyDistance;                  /* AiSiteGroup_RateSurroundings: distance to the nearest enemy unit that can reach the site */
    u8 propertyMoves;                  /* AiSiteGroup_RateSurroundings: Infantry move cost to the nearest property the team does not own; 0xff: not set */
    u8 unk_09;                         /* lowest range-map value of a current player's Infantry or Mech (func_02092134) */
    u8 unk_0a;                         /* same for a unit waiting for a T-Copter that fits a Lander */
    s8 score;                          /* set by AiSite_ScorePiperunnerNeed */
} AiSite;

typedef char AiSite_size_check[sizeof(AiSite) == 12 ? 1 : -1];

extern AiSite data_0229e5bc[];

#endif
