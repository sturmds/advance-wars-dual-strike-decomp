#ifndef GAME_AI_AI_FIND_TARGET_H
#define GAME_AI_AI_FIND_TARGET_H

#include "types.h"

void AiUnit_MoveToOtherMap(void);
void AiUnit_MoveToEnemyProperty(void);
BOOL AiUnit_PickOrderSite(s16* site);
void func_020a9e44(void);
BOOL Ai_FindReachableAiTarget(s16* out);
void func_020a9f64(void);
BOOL func_020a9f9c(s16* out);
void func_020aa068(void);
BOOL Ai_FindOwnHqTile(s16* out);
void func_020aa1ec(void);
BOOL Ai_FindNearestOwnPropertyTile(s16* out);
void func_020aa4a4(void);
BOOL AiTransport_PickFootUnit(s16* tile);
void func_020aa9c4(void);
BOOL Ai_FindCaptureSiteTile(s16* out);
void func_020aac28(void);
BOOL func_020aac60(s16* out);

#endif
