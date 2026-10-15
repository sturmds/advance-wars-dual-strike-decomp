#ifndef GAME_AI_AI_FOOT_H
#define GAME_AI_AI_FOOT_H

#include "types.h"

void AiFoot_PickAttackTarget(void);
void AiFoot_JoinCapturingUnitLoose(void);
void AiFoot_JoinCapturingUnit(void);
void AiFoot_BoardTransport(void);
void AiFoot_BoardTransportForSite(int siteDistance);
void AiFoot_HeadForCaptureSite(void);
void AiFoot_PickCaptureTarget(void);
void AiFoot_KeepCapturing(void);
void AiFoot_AttackCapturingUnit(void);

#endif
