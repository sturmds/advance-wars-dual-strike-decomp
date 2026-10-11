#include "types.h"
#include "game/co_data.h"
#include "game/play_records.h"
#include "game/Unk_02290a18.h"

extern u8 data_02291672[];
extern u8 data_02290256[];

void func_02017ca0(int limit);
void func_02017c88(int limit);
void Co_SetSavedPaletteVariant(int co, int variant);
void Co_SetSavedPictureVariant(int co, int variant);
void Co_SetAvailable(int co, int value);
void func_0210408c(void);
void Campaign_ResetProgress(void);
void CoPicks_ResetToDefaults(void);
void BattleRecord_ClearAll(void);
void func_02101b94(void);
void ForceRank_ResetAllSets(void);
void PlayRecords_ResetCounters(void);
void func_0208c010(void);
void LinkReaction_LoadTexts(void);
void Mem_ZeroBytes(u8 *dest, u32 size);

/* Sets the saved progress to its new-game values: the two limits, the
 * settings bytes of gPlayRecords, every CO's palette and picture variant and
 * availability, campaign progress, the main menu scores and the default link
 * reaction texts. The other calls reset data not yet named. */
void Save_ResetProgress(void)
{
    int co;

    func_02017ca0(0);
    func_02017c88(0);
    gPlayRecords.unk_11 = 0;
    gPlayRecords.unk_10 = 0;
    gPlayRecords.unk_12 = 1;
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
    gPlayRecords.unk_16 = 2;
    gPlayRecords.unk_17 = 2;
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
    func_02101b94();
    ForceRank_ResetAllSets();
    PlayRecords_ResetCounters();
    func_0208c010();
    LinkReaction_LoadTexts();
    gPlayRecords.unk_0f = 0;
    Mem_ZeroBytes(data_02291672, 0x100);
    Mem_ZeroBytes(data_02290256, 0x40);
}
