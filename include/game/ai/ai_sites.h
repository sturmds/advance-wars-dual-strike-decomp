#ifndef GAME_AI_AI_SITES_H
#define GAME_AI_AI_SITES_H

#include "types.h"
#include "game/map_unit.h"

BOOL AiTurn_TrySiteAction(void);
BOOL func_02091eb0(void);
int AiSites_Collect(void);
void func_02092134(void);
void AiSiteGroup_RateSurroundings(int group);
BOOL AiSite_UnitCanReach(int siteIndex, MapUnit* unit);
int AiSites_BuildGroups(void);

#endif
