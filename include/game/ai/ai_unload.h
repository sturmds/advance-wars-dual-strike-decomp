#ifndef GAME_AI_AI_UNLOAD_H
#define GAME_AI_AI_UNLOAD_H

#include "types.h"
#include "game/map_unit.h"

int AiSeaTransport_ScoreFootUnitsVsProperties(void);
BOOL AiSeaTransport_FewFootUnitsNoTarget(void);
BOOL AiSeaTransport_FindShoreTile(s16* out);
BOOL AiUnit_HasReachableTarget(void);
void func_020b5bf4(void);
void AiTransport_HeadForUnloadTile(void);
void AiTransport_HeadForFlaggedUnloadTile(void);
void AiTransport_UnloadInReach(void);
void AiTransport_UnloadAt(int x, int y);
int AiTransport_FindDropTiles(int x, int y, s16* firstDrop, s16* secondDrop);
int AiTransport_ScoreDropTile(int x, int y, MapUnit* unit);
int AiTransport_CountUnloadTiles(int x, int y);

void AiSeaTransport_ScoreUnloadTiles(void);

#endif
