#include "types.h"
#include "game/war_room_records.h"

void Survival_ClearRecords(void);

/* Campaign records by map (id - CAMPAIGN_FIRST_MAP), normal then hard. */
extern WarRoomRecord gCampaignBestRecords[][2];

#define WAR_ROOM_TABLE_COUNT 4
#define WAR_ROOM_MAP_COUNT 44
#define WAR_ROOM_PLACE_COUNT 3
#define CAMPAIGN_MAP_COUNT 28

/* Erases every War Room, Campaign and Survival record. The order of the
 * local declarations is needed to match. */
void BattleRecord_ClearAll(void) {
  int map;
  int place;
  int table;
  WarRoomRecord* record;

  for (table = 0; table < WAR_ROOM_TABLE_COUNT; table++) {
    for (map = 0; map < WAR_ROOM_MAP_COUNT; map++) {
      for (place = 0; place < WAR_ROOM_PLACE_COUNT; place++) {
        record = &gWarRoomRecords[table][map][place];
        record->frontCo = 0;
        record->backCo = 0;
        record->days = 0;
        record->score = 0;
      }
    }
  }
  for (map = 0; map < CAMPAIGN_MAP_COUNT; map++) {
    for (place = 0; place < 2; place++) {
      record = &gCampaignBestRecords[map][place];
      record->frontCo = 0;
      record->backCo = 0;
      record->days = 0;
      record->score = 0;
    }
  }
  Survival_ClearRecords();
}
