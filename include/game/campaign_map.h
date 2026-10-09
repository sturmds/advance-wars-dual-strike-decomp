#ifndef GAME_CAMPAIGN_MAP_H
#define GAME_CAMPAIGN_MAP_H

#include "types.h"

/* One campaign mission on the world map (gCampaignMissions, 12 bytes each). */
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
    u16 unk_52;                        /* 2: also checks Player1Team_HasPlayerInGame */
} Unk_0204f960;

/* World map state (gCampaignMapState). */
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

#endif
