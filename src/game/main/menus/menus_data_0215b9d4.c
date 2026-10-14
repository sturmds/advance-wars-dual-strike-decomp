#include "types.h"
#include "game/campaign_map.h"

extern u8 data_ov001_02374840[];
extern u8 data_ov001_023748a0[];
extern u8 data_ov001_02374900[];
extern u8 data_ov001_02374a30[];
extern u8 data_ov001_02374b80[];
extern u8 data_ov001_02374cf0[];
extern u8 data_ov001_02374e60[];
extern u8 data_ov001_02374fe0[];
extern u8 data_ov001_023751a0[];
extern u8 data_ov001_02375370[];
extern u8 data_ov001_02375540[];
extern u8 data_ov001_02375730[];
extern u8 data_ov001_02375920[];
extern u8 data_ov001_02375b10[];
extern u8 data_ov001_02375d20[];
extern u8 data_ov001_02375f30[];
extern u8 data_ov001_02376140[];
extern u8 data_ov001_02376370[];
extern u8 data_ov001_023765a0[];
extern u8 data_ov001_02376870[];
extern u8 data_ov001_02376b90[];
extern u8 data_ov001_02376f10[];
extern u8 data_ov001_02377290[];
extern u8 data_ov001_02377660[];
extern u8 data_ov001_02377ac0[];
extern u8 data_ov001_02377fe0[];
extern u8 data_ov001_02378520[];
extern u8 data_ov001_02378ab0[];

/* "/ohashi/res_gmap_map2" */
char data_0215b9ec[22] = {
#include "data/main/data_0215b9ec.inc.c"
};
/* "/ohashi/res_gmap_map1" */
char data_0215b9d4[22] = {
#include "data/main/data_0215b9d4.inc.c"
};
CampaignMission gCampaignMissions[29] = {
#include "data/main/gCampaignMissions.inc.c"
};
