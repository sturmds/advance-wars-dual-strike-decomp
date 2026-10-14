#include "types.h"
#include "game/Unk_022a7fac.h"
#include "game/save_progress.h"
#include "game/screen_other_map.h"
#include "game/play_records.h"

void Screen_SetCurrentBoth(void* state);
void MainMenu_SetUnk0f_4(int value);
void TileCodeTable_Load(void);
int BattleSave_GetRecordId(int gameMode, int campaignSlot);
u8 func_020193bc(int recordId);
int func_0201fe28(u8 recordId);
BOOL Profile_IsInUse(int index);
BOOL Survival_IsPastFirstMap(void);
BOOL VersusSave_CanContinue(int gameMode);

/* One byte per save record id (see BattleSave_GetRecordId); game/Unk_022a7fac.h
 * describes the same bytes as a struct. */
extern u8 data_022a7fac[];

/* Sets gameMode 5 on both screens' states, sets MainMenu.unk_0f_4 from
 * gPlayRecords.unk_10, loads the tile code table, and fills data_022a7fac:
 * for each game mode, 1 at its record id when func_020193bc gives a nonzero
 * flag for the record and func_0201fe28 accepts it. Campaign slots also count
 * when their profile is in use, survival when it is past its first map;
 * war room and versus also need VersusSave_CanContinue. */
void MainMenu_Prepare(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;
  int i;
  int gameMode;
  int slot;
  int recordId;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = 5;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = 5;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  MainMenu_SetUnk0f_4(gPlayRecords.unk_10);
  TileCodeTable_Load();

  for (i = 0; i < (int)sizeof(Unk_022a7fac); i++) {
    data_022a7fac[i] = 0;
  }

  for (gameMode = 0; gameMode <= GAME_MODE_SURVIVAL; gameMode++) {
    switch (gameMode) {
      case GAME_MODE_CAMPAIGN:
        for (slot = 0; slot < SAVE_PROFILE_COUNT; slot++) {
          recordId = BattleSave_GetRecordId(gameMode, slot);
          if ((func_020193bc(recordId) != 0 && func_0201fe28(recordId) == 0) ||
              Profile_IsInUse(slot)) {
            data_022a7fac[recordId] = 1;
          } else {
            data_022a7fac[recordId] = 0;
          }
        }
        break;
      case GAME_MODE_SURVIVAL:
        recordId = BattleSave_GetRecordId(gameMode, 0);
        if ((func_020193bc(recordId) != 0 && func_0201fe28(recordId) == 0) ||
            Survival_IsPastFirstMap()) {
          data_022a7fac[recordId] = 1;
        } else {
          data_022a7fac[recordId] = 0;
        }
        break;
      default:
        recordId = BattleSave_GetRecordId(gameMode, 0);
        if (func_020193bc(recordId) != 0 && func_0201fe28(recordId) == 0 &&
            VersusSave_CanContinue(gameMode)) {
          data_022a7fac[recordId] = 1;
        } else {
          data_022a7fac[recordId] = 0;
        }
        break;
    }
  }
}
