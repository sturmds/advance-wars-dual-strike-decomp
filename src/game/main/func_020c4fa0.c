#include "types.h"

void Players_Reset(void);
void Weather_ResetChances(void);
void Map_DrawSpecialTerrain(void);
void Players_ApplyBattleSetup(void);
void Turn_AdvanceToNextPlayer(void);
void Map_MarkCrystalObeliskRange(void);
void Map_InitTileHp(void);

void func_020c4fa0(void) {
  Players_Reset();
  Players_ApplyBattleSetup();
  Turn_AdvanceToNextPlayer();
  Weather_ResetChances();
  Map_DrawSpecialTerrain();
  Map_InitTileHp();
  Map_MarkCrystalObeliskRange();
}
