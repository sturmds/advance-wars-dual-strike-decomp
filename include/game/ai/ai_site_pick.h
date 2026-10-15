#ifndef GAME_AI_AI_SITE_PICK_H
#define GAME_AI_AI_SITE_PICK_H

#include "types.h"

BOOL AiSites_HasType(int siteType);
int AiSite_PickPortForCruiser(void);
int AiSite_PickPortAgainstSubs(void);
int AiSite_PickAirport(void);
int AiSite_PickPiperunnerSite(void);
int AiSite_PickBlackBombAirport(void);
int AiSite_PickBlackBombAirportNearestEnemy(void);
int AiSite_PickBlackBombAirportMostEnemies(void);
int AiSite_PickForBuild(int siteType);
int AiBuildSlot_PickSafeSite(void);
int AiSite_PickForFootUnit(void);
int AiSite_PickNearestToProperty(int siteType, int group);
int AiSite_PickInMostThreatenedGroup(int siteType);
int AiSite_PickBestBalance(void);
int AiSite_PickMostEnemies(int siteType, int group);
int AiSite_PickNearestEnemy(int siteType, int group);
int AiSite_PickNearestEnemyPenalized(int siteType);
int AiSite_PickForLander(int siteType);
int AiSite_PickNearestFootWithProperty(int siteType);
int AiSite_PickNearestFoot(int siteType);
int AiSite_PickNearestToPropertyAnyGroup(int siteType);
int AiSite_PickMostEnemiesAnyGroup(int siteType);
int AiSite_PickFirst(int siteType);

#endif
