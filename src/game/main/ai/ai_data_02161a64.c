#include "types.h"
#include "game/ai_unit_action.h"

void AiAction_QueueOoziums(void);
void AiAction_QueueCoPowerCheck(void);
void AiAction_QueueCapturingUnits(void);
void AiAction_QueueBlackBombExplosions(void);
void AiAction_QueueClass3Units(void);
void AiAction_QueueBlackBoats(void);
void AiAction_QueueIndirectUnits(void);
void AiAction_QueueFootUnits(void);
void AiAction_QueueAllDirectUnitsFog(void);
void AiAction_QueueDirectUnits(void);
void func_0208e89c(void);
void AiAction_QueueFootUnits2(void);
void AiAction_QueueDirectUnitsOrder4(void);
void func_0208e7dc(void);
void AiAction_QueueEmptyTransports(void);
void func_0208e77c(void);
void func_0208e8fc(void);
void AiAction_QueueTransportUnloads(void);
void AiAction_QueueLoadedTransports(void);
void AiAction_QueueIndirectMoves(void);
void AiAction_QueueSeaTransports(void);
void AiAction_QueueAllDirectUnits(void);
void AiAction_QueueBlackBombsForEagle(void);
void AiAction_QueueBlackBombs(void);
void AiAction_EndUnitActions(void);

DomainBits data_02161a64 = {
#include "data/main/data_02161a64.inc.c"
};
void (*gAiUnitPhases[])(void) = {&AiAction_QueueOoziums,
                                 &AiAction_QueueOoziums,
                                 &AiAction_QueueCoPowerCheck,
                                 &AiAction_QueueCapturingUnits,
                                 &AiAction_QueueBlackBombExplosions,
                                 &AiAction_QueueClass3Units,
                                 &AiAction_QueueBlackBoats,
                                 &AiAction_QueueIndirectUnits,
                                 &AiAction_QueueCoPowerCheck,
                                 &AiAction_QueueFootUnits,
                                 &AiAction_QueueDirectUnits,
                                 &AiAction_QueueDirectUnits,
                                 &func_0208e89c,
                                 &AiAction_QueueFootUnits2,
                                 &AiAction_QueueDirectUnitsOrder4,
                                 &func_0208e7dc,
                                 &AiAction_QueueEmptyTransports,
                                 &func_0208e77c,
                                 &func_0208e8fc,
                                 &AiAction_QueueTransportUnloads,
                                 &AiAction_QueueLoadedTransports,
                                 &AiAction_QueueIndirectMoves,
                                 &AiAction_QueueSeaTransports,
                                 &AiAction_QueueSeaTransports,
                                 &AiAction_QueueFootUnits2,
                                 &AiAction_QueueBlackBombsForEagle,
                                 &AiAction_QueueCoPowerCheck,
                                 &AiAction_QueueAllDirectUnits,
                                 &AiAction_QueueIndirectMoves,
                                 &AiAction_QueueBlackBombExplosions,
                                 &AiAction_QueueBlackBombs,
                                 &AiAction_EndUnitActions};
void (*gAiUnitPhasesFog[])(void) = {&AiAction_QueueOoziums,
                                    &AiAction_QueueOoziums,
                                    &AiAction_QueueCoPowerCheck,
                                    &AiAction_QueueCapturingUnits,
                                    &AiAction_QueueBlackBombExplosions,
                                    &AiAction_QueueClass3Units,
                                    &AiAction_QueueBlackBoats,
                                    &AiAction_QueueIndirectUnits,
                                    &AiAction_QueueCoPowerCheck,
                                    &AiAction_QueueFootUnits,
                                    &AiAction_QueueAllDirectUnitsFog,
                                    &AiAction_QueueDirectUnits,
                                    &AiAction_QueueDirectUnits,
                                    &func_0208e89c,
                                    &AiAction_QueueFootUnits2,
                                    &AiAction_QueueDirectUnitsOrder4,
                                    &func_0208e7dc,
                                    &AiAction_QueueEmptyTransports,
                                    &func_0208e77c,
                                    &func_0208e8fc,
                                    &AiAction_QueueTransportUnloads,
                                    &AiAction_QueueLoadedTransports,
                                    &AiAction_QueueIndirectMoves,
                                    &AiAction_QueueSeaTransports,
                                    &AiAction_QueueSeaTransports,
                                    &AiAction_QueueBlackBombExplosions,
                                    &AiAction_QueueIndirectUnits,
                                    &AiAction_QueueAllDirectUnits,
                                    &AiAction_QueueIndirectMoves,
                                    &AiAction_QueueFootUnits,
                                    &AiAction_QueueBlackBombsForEagle,
                                    &AiAction_QueueCoPowerCheck,
                                    &AiAction_QueueAllDirectUnits,
                                    &AiAction_QueueIndirectMoves,
                                    &AiAction_QueueFootUnits2,
                                    &AiAction_QueueBlackBombExplosions,
                                    &AiAction_QueueBlackBombs,
                                    &AiAction_EndUnitActions};
