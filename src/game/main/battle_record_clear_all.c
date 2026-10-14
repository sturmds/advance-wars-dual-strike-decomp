#include "types.h"
#include "game/war_room_records.h"

void Survival_ClearRecords(void);

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
    for (place = 0; place < CAMPAIGN_DIFFICULTY_COUNT; place++) {
      record = &gCampaignBestRecords[map][place];
      record->frontCo = 0;
      record->backCo = 0;
      record->days = 0;
      record->score = 0;
    }
  }
  Survival_ClearRecords();
}
