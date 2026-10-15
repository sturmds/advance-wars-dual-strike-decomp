#ifndef GAME_AI_AI_THREAT_MAPS_H
#define GAME_AI_AI_THREAT_MAPS_H

#include "types.h"

void AiEndActions_BuildThreatMaps(void);
void AiEndActions_AddEnemyThreat(u8 player);
void Ai_BuildFighterThreatMaps(void);
void Ai_AddEnemyThreatToFighter(u8 player);
void AiOozium_BuildThreatMaps(void);
void func_0209b2b0(int arg);
void Ai_ClearThreatInAntiAirCover(void);
void AiBlackBomb_AddEnemyThreat(u8 player, int skipEnemyCheck);

#endif
