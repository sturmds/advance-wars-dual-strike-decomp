#ifndef GAME_AI_AI_TRANSPORT_ACT_H
#define GAME_AI_AI_TRANSPORT_ACT_H

#include "types.h"

void AiUnit_MoveToEnemyTarget(void);
BOOL AiUnit_FindEnemyTarget(s16* tile);
void AiUnit_SeekSeaTransport(void);
void AiUnit_BoardSeaTransport(void);
void AiLoadedTransport_HeadForSite(void);
void AiTransport_UnloadForCapture(void);
void AiEmptyTransport_WaitNearBase(void);
void AiEmptyTransport_GoToFootUnit(void);

#endif
