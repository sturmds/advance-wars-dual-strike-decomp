#ifndef GAME_CAMPAIGN_MAP_H
#define GAME_CAMPAIGN_MAP_H

#include "types.h"

struct EventScriptCommand;

/* One campaign mission on the world map (gCampaignMissions, 12 bytes each). */
typedef struct CampaignMission {
  s16 mapId; /* MAP_CAMPAIGN_FIRST + mission */
  u16 unk_02;
  s16 x; /* position on the world map */
  s16 y;
  const struct EventScriptCommand*
      script; /* event script in ov001 that CampaignMap_StartMissionScript starts */
} CampaignMission;

/* Passed to the world map setup by the screen table at data_0215b5e8. */
typedef struct Unk_0204f960 {
  u8 pad_00[0x52];
  u16 unk_52; /* 2: also checks Player1Team_HasPlayerInGame */
} Unk_0204f960;

/* World map state (gCampaignMapState). */
/* Missions of the campaign map (CampaignMapState.flags). */
#define CAMPAIGN_MISSION_COUNT 28

typedef struct CampaignMapState {
  u8 playedMission; /* mission last started (CampaignMap_StartMissionScript) */
  u8 mission;       /* index into the mission table */
  u8 flags
      [CAMPAIGN_MISSION_COUNT]; /* per mission; bit 0: done (CampaignMap_MarkMissionDone) */
} CampaignMapState;

/* A CO of the three the world map shows, filled by CampaignMap_FindTopCos;
 * also the rows of gCampaignUnlockedCos (Campaign_UnlockTopCos). */
typedef struct CampaignTopCo {
  u8 co;    /* 0: empty */
  u8 rank;  /* Co_GetRank(co) */
  u8 score; /* save data byte 0x11a + co; sorted highest first */
  u8 pad_3;
} CampaignTopCo;

/* COs the world map shows and Campaign_UnlockTopCos unlocks; the rows of
 * gCampaignUnlockedCos are one more, the last left empty. */
#define CAMPAIGN_TOP_CO_COUNT 3
#define CAMPAIGN_UNLOCKED_CO_ROWS (CAMPAIGN_TOP_CO_COUNT + 1)

extern CampaignTopCo gCampaignUnlockedCos[];

/* Task of the world map scripts that scroll the map and move the cursor
 * (CampaignMap_InitCrossingSprites and the functions after it). The first 0x2c bytes belong
 * to the task system (Task_Create). */
typedef struct CampaignMapTask {
  u8 pad_00[0x2c];
  int fromX; /* scroll at the start of CampaignMap_EaseScroll's move */
  int fromY;
  int toX;
  int toY;
  int cursorX; /* position of the destination mission, set by CampaignMap_StartMissionScroll */
  int cursorY;
  u8 pad_44[0x4c - 0x44];
  s16 step; /* frame counter or phase of the running update function */
  u8 pad_4e[0x52 - 0x4e];
  u16 side; /* 1: the mission lies right of the middle of the screen (CampaignMap_ShowCursor's side argument) */
  u8 pad_54[0x58 - 0x54];
  int sprite0Wait; /* frames until crossing sprite 0 comes in again (CampaignMap_UpdateCrossingSprites) */
  int sprite1Wait;
  u8 pad_60[0x64 - 0x60];
  s16 unk_64; /* each script uses it its own way: crossing sprite 0 state, marker frame (CampaignMap_DrawMarkers), layer 1 offset */
  s16 unk_66; /* crossing sprite 1 state */
  s16 unk_68;
  s16 mission; /* index into gCampaignMissions */
} CampaignMapTask;

#endif
