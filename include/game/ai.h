#ifndef GAME_AI_H
#define GAME_AI_H

#include "types.h"
#include "game/map_unit.h"
#include "game/tile_point.h"

void AiAction_DebugHook(int x, int y, int action, int target, int arg);
void Ai_FlagCurrentPlayerEnterState18(void);
int AiFoot_ListReachableSilos(void);
BOOL Ai_IsTransportStopTile(int unitType, int x, int y);
BOOL AiSeaTransport_CargoReachesEnemyHq(void);
BOOL AiSeaTransport_CargoReachesHq(void);
BOOL AiSeaTransport_CargoReachesTarget(void);
BOOL AiSeaTransport_FewArmedUnitsInRange(int x, int y);
BOOL AiSeaTransport_FewArmedUnitsNoTarget(void);
BOOL AiSeaTransport_FewFootUnitsForProperties(int x, int y);
BOOL AiSeaTransport_FootUnitsOutnumberArmed(int x, int y);
BOOL AiSeaTransport_PropertiesOutweighArmed(void);
int AiSeaTransport_ScoreArmedCount(void);
void AiStep_DebugHook(void);
void AiTargetList_Add(u16 a, u16 b, u16 unitSlot, u16 c);
void AiTargetList_AddUnitsOnTiles(TilePointU16* points, int extra);
void AiTargetList_Clear(void);
BOOL AiUnit_CanUseOtherMapFrom(MapUnit* unit, s16 x, s16 y);
void Cannon_AddBestTarget(int x, int y, int damage, int kind, int variant);
void func_020b4e24(void);
void func_020b501c(void);
BOOL func_020b50bc(int x, int y);
void func_020b5138(void);
BOOL func_020b51ec(int x, int y);
void func_020b5268(void);
void func_020b5464(void);
s8 func_020bf6f4(void);
u8 Unit_CapturePoints(MapUnit* unit);

#endif
