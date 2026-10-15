#ifndef GAME_AI_AI_ACTION_QUEUE_H
#define GAME_AI_AI_ACTION_QUEUE_H

#include "types.h"

void AiAction_EndUnitActions(void);
void AiAction_QueueOoziums(void);
void AiAction_QueueBlackBombsForEagle(void);
void AiAction_QueueBlackBombs(void);
void AiAction_QueueBlackBombExplosions(void);
void AiAction_QueueBlackBoats(void);
void AiAction_QueueClass3Units(void);
void AiAction_QueueSeaTransports(void);
void AiAction_QueueLoadedTransports(void);
void AiAction_QueueTransportUnloads(void);
void AiAction_QueueEmptyTransports(void);
void func_0208e77c(void);
void func_0208e7dc(void);
void AiAction_QueueCoPowerCheck(void);
void func_0208e89c(void);
void func_0208e8fc(void);
void AiAction_QueueIndirectMoves(void);
void AiAction_QueueIndirectUnits(void);
void AiAction_QueueDirectUnitsOrder4(void);
void AiAction_QueueDirectUnits(void);
void AiAction_QueueAllDirectUnits(void);
void AiAction_QueueAllDirectUnitsFog(void);
void AiAction_QueueFootUnits2(void);
void AiAction_QueueFootUnits(void);
void AiAction_QueueCapturingUnits(void);
void AiQueue_SortSeaTransports(void);
void AiQueue_SortBlackBoats(void);
void AiQueue_SortByVision(BOOL descending);
void AiQueue_SortByMaxRange(void);
void AiQueue_SortByMovement(BOOL descending);
void AiQueue_SortByHp(void);
void AiQueue_SortAscending(int count);
void AiQueue_SortDescending(int count);

#endif
