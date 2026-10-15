#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/text_print.h"
#include "game/campaign_map_music.h"

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Number_DrawPadded(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                       u16 digitSet, int digits);
const void* Map_GetName(u16 mapId);
int CampaignRecords_GetNextNumber(void);
void Gfx_RequestLayerCopy(int layer);

extern CampaignMission gCampaignMissions[];

#define PANEL_TITLE_TEXT 0x41000000
#define SCORE_SUFFIX_TEXT \
  0x41000001 /* printed right after each top CO's score */

/* Prints the text of the world map panel on the screen of
 * data_027c0288.unk_04: the title, the score of each of the three top COs,
 * the number of the next campaign record (CampaignRecords_GetNextNumber) and the name of
 * the selected mission's map. */
void CampaignMap_DrawPanelText(void) {
  Unk_027c0280* savedGfx;
  int i;
  u16* map;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Text_PrintId(13, 7, data_027c0280->layer0Map, PANEL_TITLE_TEXT, 0, 0x300);
  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.campaignMap.topCos); i++) {
    if (gMenuState.campaignMap.topCos[i].co != 0) {
      Number_Draw(27, i * 3 + 10, data_027c0280->layer0Map,
                  gMenuState.campaignMap.topCos[i].score, 0, 0);
      Text_PrintImmediate(28, i * 3 + 10, data_027c0280->layer0Map,
                          SCORE_SUFFIX_TEXT, 0, 0);
    }
  }
  /* needed to match: the map pointer is read before each call */
  map = data_027c0280->layer0Map;
  Number_DrawPadded(2, 21, map, CampaignRecords_GetNextNumber(), 0, 0, 2);
  map = data_027c0280->layer0Map;
  Text_Print(4, 21, map,
             Map_GetName(gCampaignMissions[gCampaignMapState.mission].mapId), 0,
             0x200);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrentGfx(savedGfx);
}
