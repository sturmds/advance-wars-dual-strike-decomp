#ifndef GAME_AI_AI_BEST_DAMAGE_H
#define GAME_AI_AI_BEST_DAMAGE_H

#include "types.h"

int Ai_CoPairBestDamageVsDived(int unitType, int targetType, int powerState);
int Ai_CoPairBestDamage(int unitType, int targetType, int powerState);
int Ai_BestDamageVsDived(int unitType, int targetType, int powerState);
int Ai_BestDamage(int unitType, int targetType, int powerState);
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y);

#endif
