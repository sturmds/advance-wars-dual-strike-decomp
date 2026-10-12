#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/play_records.h"
#include "game/task.h"
#include "game/unlocks.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Campaign_IsHardMode(void);
int func_0210391c(void);
void func_020fd874(Task* parent);
void func_020fd88c(Task* parent);
void func_020fd8a4(Task* parent);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Campaign missions that set a game flag when finished. Mission 24 is the
 * last story map (CampaignMap_IsFinalMission). */
#define MAP_CAMPAIGN_MISSION_8 (MAP_CAMPAIGN_FIRST + 8)
#define MAP_CAMPAIGN_MISSION_18 (MAP_CAMPAIGN_FIRST + 18)
#define MAP_CAMPAIGN_FINAL (MAP_CAMPAIGN_FIRST + 24)

/* Game flags 33 and 34 are the campaign cleared in normal and hard mode (the
 * main menu shows a rank for each once set). */
#define GAME_FLAG_MISSION_8_CLEARED 35
#define GAME_FLAG_MISSION_18_CLEARED 36
#define GAME_FLAG_CAMPAIGN_CLEAR 33
#define GAME_FLAG_CAMPAIGN_HARD_CLEAR 34

#define RECORD_MAX 999999

/* Task step at the end of a campaign map: finishing mission 8, mission 18 or
 * the last story map sets its game flag and starts a child task (one script per
 * mission). Clearing the last map also counts the clear in gPlayRecords,
 * normal or hard, unless func_0210391c returns nonzero. Other game modes
 * and maps do nothing. */
void CampaignTask_SetClearFlags(Task* task) {
  if (STATE->gameMode != GAME_MODE_CAMPAIGN) {
    return;
  }
  switch (STATE->unk_1c4->mapId) {
    case MAP_CAMPAIGN_MISSION_8:
      GameFlag_Set(GAME_FLAG_MISSION_8_CLEARED, 1);
      func_020fd874(task);
      break;
    case MAP_CAMPAIGN_MISSION_18:
      GameFlag_Set(GAME_FLAG_MISSION_18_CLEARED, 1);
      func_020fd88c(task);
      break;
    case MAP_CAMPAIGN_FINAL:
      if (!Campaign_IsHardMode()) {
        GameFlag_Set(GAME_FLAG_CAMPAIGN_CLEAR, 1);
        if (!func_0210391c()) {
          if (gPlayRecords.unk_388 < RECORD_MAX) {
            gPlayRecords.unk_388++;
          }
        }
      } else {
        GameFlag_Set(GAME_FLAG_CAMPAIGN_HARD_CLEAR, 1);
        if (!func_0210391c()) {
          if (gPlayRecords.unk_38c < RECORD_MAX) {
            gPlayRecords.unk_38c++;
          }
        }
      }
      func_020fd8a4(task);
      break;
  }
}
