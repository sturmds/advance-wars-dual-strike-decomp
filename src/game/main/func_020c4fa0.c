#include "types.h"

void Players_Reset(void);
void Weather_ResetCooldowns(void);
void func_020da650(void);
void Players_ApplyBattleSetup(void);
void Turn_AdvanceToNextPlayer(void);
void Map_MarkCrystalObeliskRange(void);
void Map_InitTileHp(void);

void func_020c4fa0(void)
{
    Players_Reset();
    Players_ApplyBattleSetup();
    Turn_AdvanceToNextPlayer();
    Weather_ResetCooldowns();
    func_020da650();
    Map_InitTileHp();
    Map_MarkCrystalObeliskRange();
}
