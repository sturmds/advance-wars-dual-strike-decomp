#include "types.h"
#include "game/skills.h"
#include "game/play_records.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02290118.h"
#include "game/save_progress.h"

/* Needed to match: the original copies each 4-skill set as one 4-byte unit. */
typedef struct SkillSet {
  u8 id[4];
} SkillSet;

int Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
void Co_SetExperience(int co, u32 experience);
u8* ForceRank_GetSavedSet(int mode, int co, int set);

/* Loads save record 0 into data_02291774 and copies it into the live progress
 * data: two limits (PlayRecords_CapPoints, func_02017c88), every CO's experience and
 * its Force Rank sets. The sets copied depend on the game mode (0 to 3). Mode 2
 * asks ForceRank_GetSavedSet for mode 1 four times, as the original code does,
 * so its four sets all land in the mode 1 list. */
void Save_LoadProgress(void) {
  int set;
  int co;
  u8* saved;
  u8* src;
  SaveProgress* progress = &data_02291774.progress;

  if (Save_LoadRecord(0, progress)) {
    return;
  }
  PlayRecords_CapPoints(progress->unk_58);
  func_02017c88(progress->unk_5c);
  for (co = 0; co < CO_COUNT; co++) {
    Co_SetExperience(co, progress->coRecords[co].experience);
    switch (data_027c027c->unk_48[data_027c0284]->gameMode) {
      case 0:
        src = progress->profileSets[data_02290118.unk_00][co];
        saved = ForceRank_GetSavedSet(0, co, 0);
        *(SkillSet*)saved = *(SkillSet*)src;
        break;
      case 1:
        saved = ForceRank_GetSavedSet(1, co, 0);
        src = progress->coRecords[co].setMode1;
        *(SkillSet*)saved = *(SkillSet*)src;
        break;
      case 2:
        for (set = 0; set < FORCE_RANK_SET_COUNT; set++) {
          saved = ForceRank_GetSavedSet(1, co, set);
          src = progress->coRecords[co].setsMode2[set];
          *(SkillSet*)saved = *(SkillSet*)src;
        }
        break;
      case 3:
        saved = ForceRank_GetSavedSet(3, co, 0);
        src = progress->coRecords[co].setMode3;
        *(SkillSet*)saved = *(SkillSet*)src;
        break;
    }
  }
}
