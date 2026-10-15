#ifndef GAME_AI_AI_BUILD_PICK_H
#define GAME_AI_AI_BUILD_PICK_H

#include "types.h"

BOOL Ai_HasHealthyFighter(void);
BOOL Ai_EnemyHasAirUnit(void);
BOOL Ai_HasOwnBaseNearHq(void);
int AiSite_ChooseUnitToBuild(int siteIndex);
int AiTurn_PickUnitToBuild(int allowSaving);
void AiSite_ScorePiperunnerNeed(void);
BOOL AiBuild_CanAffordPiperunnerInstead(int unitType);
BOOL AiBuild_WantsPiperunnerInstead(int unitType);
BOOL Ai_WantsBlackBomb(int arg);
BOOL AiBuild_WantsBlackBombInstead(int unitType);
void AiBuild_SumUnitHp(void);
void AiSite_SumNearbyUnitHp(int siteIndex);
void AiBuild_WeighUnitTypes(void);
int AiBuild_PickAntiAir(void);
BOOL AiProfile_AllowsBuild(u8 unitType);
int AiBuild_RateAttackerVsTarget(u8 attacker, int target);
int AiBuild_ScaleByReach(int score, int unitType, int powerState);
int AiBuild_GetTargetGroup(u8 unitType);

#endif
