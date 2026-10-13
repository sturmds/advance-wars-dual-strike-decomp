#include "types.h"
#include "game/co_data.h"
#include "game/play_records.h"
#include "game/Unk_02290a18.h"
#include "game/visual_mode.h"

extern u8 data_02291672[];
extern u8 data_02290256[];

void Co_SetSavedPaletteVariant(int co, int variant);
void Co_SetSavedPictureVariant(int co, int variant);
void Co_SetAvailable(int co, int value);
void func_0210408c(void);
void Campaign_ResetProgress(void);
void CoPicks_ResetToDefaults(void);
void BattleRecord_ClearAll(void);
void Unlocks_ResetToNewGame(void);
void ForceRank_ResetAllSets(void);
void PlayRecords_ResetCounters(void);
void func_0208c010(void);
void LinkReaction_LoadTexts(void);
void Mem_ZeroBytes(u8* dest, u32 size);

/* Sets the saved progress to its new-game values: the two limits, the
 * settings bytes of gPlayRecords, every CO's palette and picture variant and
 * availability, campaign progress, the main menu scores and the default link
 * reaction texts. The other calls reset data not yet named. */
void Save_ResetProgress(void) {
  int co;

  PlayRecords_CapPoints(0);
  func_02017c88(0);
  gPlayRecords.unk_11 = 0;
  gPlayRecords.unk_10 = 0;
  gPlayRecords.visualMode = VISUAL_A;
  gPlayRecords.unk_0e = 1;
  gPlayRecords.unk_08 = 0;
  gPlayRecords.unk_09 = 0;
  gPlayRecords.unk_0a = 0;
  gPlayRecords.unk_0b = 0;
  gPlayRecords.unk_0c = 0;
  gPlayRecords.unk_0d = 0;
  gPlayRecords.shopVisits = 0;
  gPlayRecords.wallpaper = 0;
  gPlayRecords.unk_15 = 2;
  gPlayRecords.warRoomAiBehaviour = 2;
  gPlayRecords.campaignAiBehaviour = 2;
  for (co = 0; co < CO_COUNT; co++) {
    Co_SetSavedPaletteVariant(co, 0);
    Co_SetSavedPictureVariant(co, 0);
    Co_SetAvailable(co, FALSE);
  }
  func_0210408c();
  Campaign_ResetProgress();
  data_02290a18.unk_78 = 0;
  data_02290a18.unk_7a = 0;
  CoPicks_ResetToDefaults();
  BattleRecord_ClearAll();
  Unlocks_ResetToNewGame();
  ForceRank_ResetAllSets();
  PlayRecords_ResetCounters();
  func_0208c010();
  LinkReaction_LoadTexts();
  gPlayRecords.unk_0f = 0;
  Mem_ZeroBytes(data_02291672, 0x100);
  Mem_ZeroBytes(data_02290256, 0x40);
}
