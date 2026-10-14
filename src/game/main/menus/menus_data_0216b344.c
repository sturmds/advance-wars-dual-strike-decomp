#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/unlocks.h"

void Shop_BeatCampaign(void);
void Shop_BeatHardCampaign(void);
void Shop_BeatMoneySurvival(void);
void Shop_BeatTimeSurvival(void);
void Shop_BeatTurnSurvival(void);
void Shop_CombatAdder(void);
void Shop_CombatFlak(void);
void Shop_CombatHawke(void);
void Shop_CombatLash(void);
void Shop_CombatLevel(void);
void Shop_Gallery(void);
void Shop_GalleryAw1(void);
void Shop_GalleryAw2(void);
void Shop_HardCampaign(void);
void Shop_MapAw1(void);
void Shop_MapAw2(void);
void Shop_MapVersus(void);
void Shop_MapWarRoom(void);
void Shop_Oozium(void);
void Shop_SmashedCrystal(void);
void Shop_SmashedObelisk(void);
void Shop_SoundRoom(void);
void Shop_UnlockedCo(void);
void func_ov005_02367eac(void);
void Map_SetFlag(void);
void Map_SetSingleFlag(void);

char data_0216b35c[] = {
#include "data/main/data_0216b35c.inc.c"
};
char data_0216b350[] = {
#include "data/main/data_0216b350.inc.c"
};
char data_0216b34c[] = {
#include "data/main/data_0216b34c.inc.c"
};
char data_0216b354[] = {
#include "data/main/data_0216b354.inc.c"
};
char data_0216b358[] = {
#include "data/main/data_0216b358.inc.c"
};
char data_0216b348[] = {
#include "data/main/data_0216b348.inc.c"
};
char data_0216b344[] = {
#include "data/main/data_0216b344.inc.c"
};
u32 data_0216c960[] = {
#include "data/main/data_0216c960.inc.c"
};
u32 data_0216c920[] = {
#include "data/main/data_0216c920.inc.c"
};
u32 data_0216c8e0[] = {
#include "data/main/data_0216c8e0.inc.c"
};
u32 data_0216c8a0[] = {
#include "data/main/data_0216c8a0.inc.c"
};
u32 data_0216c860[] = {
#include "data/main/data_0216c860.inc.c"
};
u32 data_0216c820[] = {
#include "data/main/data_0216c820.inc.c"
};
u32 data_0216c7e0[] = {
#include "data/main/data_0216c7e0.inc.c"
};
u32 data_0216c7a0[] = {
#include "data/main/data_0216c7a0.inc.c"
};
u32 data_0216c760[] = {
#include "data/main/data_0216c760.inc.c"
};
u32 data_0216c720[] = {
#include "data/main/data_0216c720.inc.c"
};
u32 data_0216c6e0[] = {
#include "data/main/data_0216c6e0.inc.c"
};
u32 data_0216c6a0[] = {
#include "data/main/data_0216c6a0.inc.c"
};
u32 data_0216c660[] = {
#include "data/main/data_0216c660.inc.c"
};
u32 data_0216c620[] = {
#include "data/main/data_0216c620.inc.c"
};
u32 data_0216c5e0[] = {
#include "data/main/data_0216c5e0.inc.c"
};
u32 data_0216c5a0[] = {
#include "data/main/data_0216c5a0.inc.c"
};
u32 data_0216c560[] = {
#include "data/main/data_0216c560.inc.c"
};
u32 data_0216c520[] = {
#include "data/main/data_0216c520.inc.c"
};
u32 data_0216c4e0[] = {
#include "data/main/data_0216c4e0.inc.c"
};
u32 data_0216c4a0[] = {
#include "data/main/data_0216c4a0.inc.c"
};
u32 data_0216c460[] = {
#include "data/main/data_0216c460.inc.c"
};
u32 data_0216c420[] = {
#include "data/main/data_0216c420.inc.c"
};
u32 data_0216c3e0[] = {
#include "data/main/data_0216c3e0.inc.c"
};
u32 data_0216c3a0[] = {
#include "data/main/data_0216c3a0.inc.c"
};
u32 data_0216c360[] = {
#include "data/main/data_0216c360.inc.c"
};
u32 data_0216c320[] = {
#include "data/main/data_0216c320.inc.c"
};
u32 data_0216c2e0[] = {
#include "data/main/data_0216c2e0.inc.c"
};
u32 data_0216c2a0[] = {
#include "data/main/data_0216c2a0.inc.c"
};
u32 data_0216c260[] = {
#include "data/main/data_0216c260.inc.c"
};
u32 data_0216c220[] = {
#include "data/main/data_0216c220.inc.c"
};
u32 data_0216c160[] = {
#include "data/main/data_0216c160.inc.c"
};
u32 data_0216c1a0[] = {
#include "data/main/data_0216c1a0.inc.c"
};
u32 data_0216c9a0[] = {
#include "data/main/data_0216c9a0.inc.c"
};
u32 data_0216c9e0[] = {
#include "data/main/data_0216c9e0.inc.c"
};
u32 data_0216c0e0[] = {
#include "data/main/data_0216c0e0.inc.c"
};
u32 data_0216c0a0[] = {
#include "data/main/data_0216c0a0.inc.c"
};
u32 data_0216c060[] = {
#include "data/main/data_0216c060.inc.c"
};
u32 data_0216c020[] = {
#include "data/main/data_0216c020.inc.c"
};
u32 data_0216bfe0[] = {
#include "data/main/data_0216bfe0.inc.c"
};
u32 data_0216bfa0[] = {
#include "data/main/data_0216bfa0.inc.c"
};
u32 data_0216bf60[] = {
#include "data/main/data_0216bf60.inc.c"
};
u32 data_0216bf20[] = {
#include "data/main/data_0216bf20.inc.c"
};
u32 data_0216bee0[] = {
#include "data/main/data_0216bee0.inc.c"
};
u32 data_0216bea0[] = {
#include "data/main/data_0216bea0.inc.c"
};
u32 data_0216be60[] = {
#include "data/main/data_0216be60.inc.c"
};
u32 data_0216be20[] = {
#include "data/main/data_0216be20.inc.c"
};
u32 data_0216bde0[] = {
#include "data/main/data_0216bde0.inc.c"
};
u32 data_0216bda0[] = {
#include "data/main/data_0216bda0.inc.c"
};
u32 data_0216bd60[] = {
#include "data/main/data_0216bd60.inc.c"
};
u32 data_0216bd20[] = {
#include "data/main/data_0216bd20.inc.c"
};
u32 data_0216bce0[] = {
#include "data/main/data_0216bce0.inc.c"
};
u32 data_0216bca0[] = {
#include "data/main/data_0216bca0.inc.c"
};
u32 data_0216bc60[] = {
#include "data/main/data_0216bc60.inc.c"
};
u32 data_0216bc20[] = {
#include "data/main/data_0216bc20.inc.c"
};
u32 data_0216bbe0[] = {
#include "data/main/data_0216bbe0.inc.c"
};
u32 data_0216bba0[] = {
#include "data/main/data_0216bba0.inc.c"
};
u32 data_0216bb60[] = {
#include "data/main/data_0216bb60.inc.c"
};
u32 data_0216bb20[] = {
#include "data/main/data_0216bb20.inc.c"
};
u32 data_0216bae0[] = {
#include "data/main/data_0216bae0.inc.c"
};
u32 data_0216baa0[] = {
#include "data/main/data_0216baa0.inc.c"
};
u32 data_0216ba60[] = {
#include "data/main/data_0216ba60.inc.c"
};
u32 data_0216ba20[] = {
#include "data/main/data_0216ba20.inc.c"
};
u32 data_0216b9a0[] = {
#include "data/main/data_0216b9a0.inc.c"
};
u32 data_0216b9e0[] = {
#include "data/main/data_0216b9e0.inc.c"
};
u32 data_0216c1e0[] = {
#include "data/main/data_0216c1e0.inc.c"
};
u32 data_0216ca20[] = {
#include "data/main/data_0216ca20.inc.c"
};
u32 data_0216b8e0[] = {
#include "data/main/data_0216b8e0.inc.c"
};
u32 data_0216b8a0[] = {
#include "data/main/data_0216b8a0.inc.c"
};
u32 data_0216b860[] = {
#include "data/main/data_0216b860.inc.c"
};
u32 data_0216b820[] = {
#include "data/main/data_0216b820.inc.c"
};
u32 data_0216b7e0[] = {
#include "data/main/data_0216b7e0.inc.c"
};
u32 data_0216b7a0[] = {
#include "data/main/data_0216b7a0.inc.c"
};
u32 data_0216b760[] = {
#include "data/main/data_0216b760.inc.c"
};
u32 data_0216b720[] = {
#include "data/main/data_0216b720.inc.c"
};
u32 data_0216b6e0[] = {
#include "data/main/data_0216b6e0.inc.c"
};
u32 data_0216b6a0[] = {
#include "data/main/data_0216b6a0.inc.c"
};
u32 data_0216b660[] = {
#include "data/main/data_0216b660.inc.c"
};
u32 data_0216b620[] = {
#include "data/main/data_0216b620.inc.c"
};
u32 data_0216b5e0[] = {
#include "data/main/data_0216b5e0.inc.c"
};
u32 data_0216b920[] = {
#include "data/main/data_0216b920.inc.c"
};
u32 data_0216c120[] = {
#include "data/main/data_0216c120.inc.c"
};
u32 data_0216ca60[] = {
#include "data/main/data_0216ca60.inc.c"
};
u32 data_0216b4e0[] = {
#include "data/main/data_0216b4e0.inc.c"
};
u32 data_0216b4a0[] = {
#include "data/main/data_0216b4a0.inc.c"
};
u32 data_0216b460[] = {
#include "data/main/data_0216b460.inc.c"
};
u32 data_0216b420[] = {
#include "data/main/data_0216b420.inc.c"
};
u32 data_0216b520[] = {
#include "data/main/data_0216b520.inc.c"
};
u32 data_0216b560[] = {
#include "data/main/data_0216b560.inc.c"
};
u32 data_0216b960[] = {
#include "data/main/data_0216b960.inc.c"
};
u32 data_0216caa0[] = {
#include "data/main/data_0216caa0.inc.c"
};
u32 data_0216b360[] = {
#include "data/main/data_0216b360.inc.c"
};
u32 data_0216b3a0[] = {
#include "data/main/data_0216b3a0.inc.c"
};
u32 data_0216b5a0[] = {
#include "data/main/data_0216b5a0.inc.c"
};
u32 data_0216cae0[] = {
#include "data/main/data_0216cae0.inc.c"
};
u32 data_0216b3e0[] = {
#include "data/main/data_0216b3e0.inc.c"
};
u32 data_0216cb20[] = {
#include "data/main/data_0216cb20.inc.c"
};
ShopItem data_0216cb60[] = {
#include "data/main/data_0216cb60.inc.c"
};
