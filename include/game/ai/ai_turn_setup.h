#ifndef GAME_AI_AI_TURN_SETUP_H
#define GAME_AI_AI_TURN_SETUP_H

#include "types.h"
#include "game/ai_profile.h"

void AiTurn_BuildCaptureMap(void);
void AiProfile_RerandomiseForPlayer(int player);
void AiProfile_RerollThreatLimits(AiProfile* profile, int modsId,
                                  int templateId);
void AiProfile_BuildForPlayer(int player);
void AiProfile_Build(AiProfile* profile, int modsId, int templateId);
int AiProfile_Randomise(u8 base, u8 spread, int roll);
void AiTurn_FindUnitDomains(void);
void AiTurn_UpdateUnitModes(void);

#endif
