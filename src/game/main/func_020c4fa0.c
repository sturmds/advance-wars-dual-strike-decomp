#include "types.h"

void Players_Reset(void);
void func_020d2194(void);
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
    func_020d2194();
    func_020da650();
    Map_InitTileHp();
    Map_MarkCrystalObeliskRange();
}
