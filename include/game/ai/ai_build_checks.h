#ifndef GAME_AI_AI_BUILD_CHECKS_H
#define GAME_AI_AI_BUILD_CHECKS_H

#include "types.h"

int AiSite_NearUnguardedOozium(int x, int y);
int AiTile_HasAdjacentOwnTeamUnit(int x, int y);
int AiTile_HasOwnTeamUnit(int x, int y);
int AiTile_HasEnemyOozium(int x, int y);
void Ai_BuildEnemySubReachMap(void);
void Ai_MarkEnemySubReach(u8 player);
void Ai_CountEnemySubReach(void);
void Ai_AddSubReach(u8 player);
int AiBuild_RateSiteDanger(int x, int y, int unitType);
int AiSite_RateBaseLandmass(void);
int AiSite_RateLandmass(int x, int y);
BOOL AiBuild_ShouldSaveFunds(u32 cost);
BOOL AiBuild_HasEnoughGroundIndirects(int flags);
BOOL AiBuild_HasEnoughIndirectsForArtillery(int flags);
BOOL AiProfile_WantsSeaTransport(void);
BOOL AiProfile_HasBuildFlag(int mask);

#endif
