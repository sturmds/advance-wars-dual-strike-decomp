#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7a9c.h"
#include "game/Unk_022a7b80.h"
#include "game/play_records.h"
#include "game/survival_record.h"
#include "game/war_room_records.h"
#include "game/co_data.h"
#include "game/math.h"

int Campaign_IsHardMode(void);
int Survival_GetRecordIndex(int mode, int index);

extern Unk_022a7b80 data_022a7b80;
/* Campaign records by map (id - CAMPAIGN_FIRST_MAP), normal then hard. */

#define CAMPAIGN_FIRST_MAP 224

/* CO ids of the winning slot's result row. */
#define FRONT_CO(slot) (data_022a7ab8[slot].unk_00 & CO_ID_MASK)
#define BACK_CO(slot) (data_022a7aba[slot].unk_00 & CO_ID_MASK)
#define WAR_ROOM_RECORD(map, place) \
  (gWarRoomRecords[MAP_STATE->unk_59][map][place])
#define CAMPAIGN_RECORD(map) \
  (gCampaignBestRecords[(map) - CAMPAIGN_FIRST_MAP][Campaign_IsHardMode()])

/* Saves the result of a won battle when it beats the stored record.
 * Survival keeps one record per map and mode: lowest time in seconds,
 * lowest funds spent, or fewest days. The War Room keeps three places
 * per map, by score and then by fewest days; a new place pushes the
 * lower ones down. The Campaign keeps one per map and difficulty, by
 * score. Each record also keeps the two COs of the winning slot. */
void BattleRecord_SaveWin(void) {
  int slot = data_022a7a9c.player;
  int mapId = MAP_STATE->unk_1c4->mapId;
  int place;
  int j;
  u32 value;
  int index;

  switch (MAP_STATE->gameMode) {
    case GAME_MODE_SURVIVAL:
      index =
          Survival_GetRecordIndex(data_022a7b80.mode, data_022a7b80.mapIndex);
      switch (data_022a7b80.mode) {
        case 0:
        case 3:
          value = HwDiv32(MAP_STATE->players[slot].playTimeFrames, 60);
          if (data_02291588[index].mode0Co != 0 &&
              data_02291650[index] <= value) {
            return;
          }
          data_02291650[index] = value;
          data_02291588[index].mode0Co = FRONT_CO(slot);
          data_02291588[index].mode0TagCo = BACK_CO(slot);
          break;
        case 1:
        case 4:
          value = MAP_STATE->players[slot].unk_04;
          if (data_0229158c[index].mode1Co != 0 &&
              data_02291584[index].fundsSpent <= value) {
            return;
          }
          data_02291584[index].fundsSpent = value;
          data_0229158c[index].mode1Co = FRONT_CO(slot);
          data_0229158c[index].mode1TagCo = BACK_CO(slot);
          break;
        case 2:
        case 5:
          value = MAP_STATE->day;
          if (data_02291588[index].mode2Co != 0 &&
              data_02291588[index].days <= value) {
            return;
          }
          data_02291588[index].days = value;
          data_02291588[index].mode2Co = FRONT_CO(slot);
          data_02291588[index].mode2TagCo = BACK_CO(slot);
          break;
      }
      break;
    case GAME_MODE_WAR_ROOM:
      mapId -= WAR_ROOM_FIRST_MAP;
      for (place = 0; place < WAR_ROOM_PLACE_COUNT; place++) {
        if (WAR_ROOM_RECORD(mapId, place).score < data_022a7a9c.score) {
          break;
        }
        if (data_022a7a9c.score == WAR_ROOM_RECORD(mapId, place).score &&
            WAR_ROOM_RECORD(mapId, place).days > MAP_STATE->day) {
          break;
        }
      }
      if (place == WAR_ROOM_PLACE_COUNT) {
        return;
      }
      /* Needed to match: the original reaches the records through
         * gPlayRecords here and through gWarRoomRecords everywhere else. */
      for (j = 1; j >= place; j--) {
        gPlayRecords.warRoomRecords[MAP_STATE->unk_59][mapId][j + 1] =
            gPlayRecords.warRoomRecords[MAP_STATE->unk_59][mapId][j];
      }
      WAR_ROOM_RECORD(mapId, place).frontCo = FRONT_CO(slot);
      WAR_ROOM_RECORD(mapId, place).backCo = BACK_CO(slot);
      WAR_ROOM_RECORD(mapId, place).score = data_022a7a9c.score;
      WAR_ROOM_RECORD(mapId, place).days = MAP_STATE->day;
      break;
    case GAME_MODE_CAMPAIGN:
      if (CAMPAIGN_RECORD(mapId).score > data_022a7a9c.score) {
        return;
      }
      CAMPAIGN_RECORD(mapId).frontCo = FRONT_CO(slot);
      CAMPAIGN_RECORD(mapId).backCo = BACK_CO(slot);
      CAMPAIGN_RECORD(mapId).score = data_022a7a9c.score;
      CAMPAIGN_RECORD(mapId).days = MAP_STATE->day;
      break;
  }
}
