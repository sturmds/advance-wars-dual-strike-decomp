#ifndef GAME_CAMPAIGN_MAP_H
#define GAME_CAMPAIGN_MAP_H

#include "types.h"

/* One campaign mission on the world map (data_0215ba04, 12 bytes each). */
typedef struct CampaignMission {
    s16 mapId;                         /* MAP_CAMPAIGN_FIRST + mission */
    u16 unk_02;
    s16 x;                             /* position on the world map */
    s16 y;
    const void *unk_08;
} CampaignMission;

/* Passed to the world map setup by the screen table at data_0215b5e8. */
typedef struct Unk_0204f960 {
    u8 pad_00[0x52];
    u16 unk_52;                        /* 2: also checks func_020d5d2c */
} Unk_0204f960;

/* World map state (data_0229c748). */
typedef struct CampaignMapState {
    u8 unk_00;
    u8 mission;                        /* index into the mission table */
} CampaignMapState;

/* A CO of the three the world map shows, filled by CampaignMap_FindTopCos. */
typedef struct CampaignTopCo {
    u8 co;                             /* 0: empty */
    u8 unk_1;                          /* Co_GetRank(co) */
    u8 score;                          /* save data byte 0x11a + co; sorted highest first */
    u8 pad_3;
} CampaignTopCo;

/* Layout of the screen work area data_0229eb30 while the world map is
 * up. Other screens use the same memory with another layout (see
 * Unk_02066818). */
typedef struct CampaignMapWork {
    u8 unk_000[0x20];                  /* handed to func_0200bb18 */
    u8 unk_020[0xc4];                  /* the three blocks are copied from res_gmap */
    u8 unk_0e4[0x160];
    u8 unk_244[0x8c];
    u8 pad_2d0[0x2d8 - 0x2d0];
    CampaignTopCo topCos[3];
    u8 enemyCos[2];                    /* front and back CO of the map's last player slot, probably the enemy */
} CampaignMapWork;

#endif
