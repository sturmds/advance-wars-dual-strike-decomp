#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/war_room_records.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/screen_other_map.h"
#include "game/campaign_map_music.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueDefault(int x, int y, int attr, int list);
void Co_QueueSmallPictureSprite(int x, int y, int attr, int list);
void CampaignMap_QueueRankNumber(int x, int y, int value);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);
int func_0210391c(void);
int Campaign_IsHardMode(void);
int Score_ToRank(int score); /* 2 below 200, 3 below 250, 4 below 280, else 5 */

extern CampaignMission gCampaignMissions[];

#define CAMPAIGN_FIRST_MAP 224
#define X_FLIP 0x1000 /* sprite x bit: flip the sprite horizontally */
#define TOP_CO(i) (gMenuState.campaignMap.topCos[i])
#define MISSION_RECORD                                                       \
  (gCampaignBestRecords[gCampaignMissions[gCampaignMapState.mission].mapId - \
                        CAMPAIGN_FIRST_MAP][Campaign_IsHardMode()])

/* Update function of the world map panel: a row for each of the three top
 * COs (frame, small picture, rank number), the enemy CO pictures, the
 * cycling highlight colour, and the rank of the current mission's record
 * for this difficulty when it has one. */
void CampaignMap_DrawCoPanel(CampaignMapTask* task) {
  Unk_027c0280* savedGfx;
  int i;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (i = 0; i < CAMPAIGN_TOP_CO_COUNT; i++) {
    Sprite_QueueAt(0, 24, (u8)(i * 24 + 80), &data_027c0060, 0x2000,
                   i * 8 + 38);
    if (TOP_CO(i).co != 0) {
      Sprite_QueueAt(0, 56, (u8)(i * 24 + 80), &data_027c00b8, 26, 0);
      /* tiles 62 + 12 * i, palette 4 + i */
      Co_QueueSmallPictureSprite(80, (u8)(i * 24 + 96),
                                 (i * 12 + 62) | ((i + 4) << 12), 1);
      CampaignMap_QueueRankNumber(108, i * 24 + 85, TOP_CO(i).rank);
      Sprite_QueueDefault(153, (u8)(i * 24 + 81), (i * 12 + 98) | 0x1000, 0);
    }
  }
  if (gMenuState.campaignMap.enemyCos[1] == 0) {
    Sprite_QueueAt(0, 184 | X_FLIP, 168, &data_027c00b8, 26, 0);
    Co_QueueSmallPictureSprite(208 | X_FLIP, 184, 0x7086, 1);
  } else {
    Sprite_QueueAt(0, 136, 168, &data_027c00b8, 26, 0);
    Co_QueueSmallPictureSprite(160 | X_FLIP, 184, 0x7086, 1);
    Sprite_QueueAt(0, 184, 168, &data_027c00b8, 26, 0);
    Co_QueueSmallPictureSprite(208 | X_FLIP, 184, 0x8092, 1);
  }
  /* one colour of the 16 every 4 frames */
  Palette_Load(&gMenuState.colours[(MainLoop_GetFrameCount() >> 2) & 15], 600,
               2);
  if (func_0210391c()) {
    Sprite_QueueAt(0, 216, 56, &data_027c0040, 0x2000, 174);
  }
  if (MISSION_RECORD.days != 0) {
    Sprite_QueueAt(
        0, 232, 168, &data_027c0040,
        ((5 - Score_ToRank(MISSION_RECORD.score)) * 4 + 158) | 0x3000, 0);
  }
  Screen_SetCurrentGfx(savedGfx);
}
