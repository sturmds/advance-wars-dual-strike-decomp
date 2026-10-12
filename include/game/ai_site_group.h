#ifndef GAME_AI_SITE_GROUP_H
#define GAME_AI_SITE_GROUP_H

#include "types.h"

/* One 4-byte entry of gAiSiteGroups (count in AiBuildState.slotCount): the
 * AI sites an Infantry can reach from the group's first site
 * (AiSite.group). Filled by AiSites_BuildGroups. */
typedef struct AiSiteGroup {
  u8 threat; /* +2 per enemy unit in reach that can attack or carries a unit, -1 per own-team unit in reach that has a weapon */
  u8 unk_01; /* cleared by func_020937dc; 0xff: used up (AiBuildSlot_PickSafeSite) */
  u8 propertyCount; /* properties in reach not owned by the current team */
  u8 skipFoot
      : 1; /* set when an own foot unit above 60 hp or an enemy non-foot ground unit is in reach, or no property to take; AiTurn_TryBuildFootUnit sets it when it builds and checks it */
  u8 needAntiAir
      : 1; /* an enemy air unit (not a T-Copter) is in reach and no own Anti-Air or Fighter above 60 hp; func_02091eb0 clears it */
  u8 unk_03_2 : 1; /* from AiSite_RateLandmass at the first site */
  u8 pad_03_3 : 5;
} AiSiteGroup;

typedef char AiSiteGroup_size_check[sizeof(AiSiteGroup) == 4 ? 1 : -1];

extern AiSiteGroup gAiSiteGroups[];

#endif
