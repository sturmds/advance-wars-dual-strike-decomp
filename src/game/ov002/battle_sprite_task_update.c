#include "types.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

u32 HwDiv32(u32 num, u32 den);
void Task_ResumeScript(BattleSpriteTask* task);
void Battle_StartSideShake(int side);
void Battle_DrawSpriteCells(u32 frame, int x, int y, int z, int unk_48,
                            int unk_4a, int unk_4c, int unk_4e, int unk_50,
                            int unk_54, int unk_56, int unk_59, int unk_5a);
void func_ov002_0235f9d8(void);
void Battle_PlayFighterSe(int side);
void Battle_PlayDefaultFighterSe(int side);
void Battle_PlayFighterReactSe(int side);
void Battle_PlayFighterStartSe(int side);

#define SHOWN_HP(hp) ((hp) == 0 ? 0 : HwDiv32((hp) - 1, 10) + 1)

/* Figures of the task's side: half its shown HP, rounded up. */
#define OWN_FIGURES(task)                                                      \
  ((task)->unk_5a == Ov002_gBattleScene->attackerSide                          \
       ? (SHOWN_HP(Ov002_gBattleScene->sides[(task)->unk_5a].hpBefore) + 1) >> \
             1                                                                 \
       : (SHOWN_HP(Ov002_gBattleScene->sides[(task)->unk_5a].hpAfter) + 1) >>  \
             1)

/* The same for the other side, with before and after swapped. */
#define OTHER_FIGURES(task)                                                   \
  ((task)->unk_5a == Ov002_gBattleScene->attackerSide                         \
       ? (SHOWN_HP(                                                           \
              Ov002_gBattleScene->sides[((task)->unk_5a + 1) & 1].hpAfter) +  \
          1) >>                                                               \
             1                                                                \
       : (SHOWN_HP(                                                           \
              Ov002_gBattleScene->sides[((task)->unk_5a + 1) & 1].hpBefore) + \
          1) >>                                                               \
             1)

/* Per-frame callback of a BattleSpriteTask: draws the current frame (again at the second
 * position when unk_57 is set), then reads the script. Each u16 entry is a command in the high
 * byte and an argument in the low byte:
 *   0xff: end, resume the task's script; 0xfe: set unk_5c; 0xfd: jump back to the start;
 *   0xfc: a sound cue or other action by the argument, else store the argument in unk_5b;
 *   other: show frame (high byte) of the unk_2c table until entryFrames reaches the argument.
 * Cues 0x25-0x29 and 0x2a-0x2e play only when the side, or the other side, has 5 to 1
 * figures. Needed to match: unk_48, unk_4a and unk_4c are read as signed. */
void BattleSpriteTask_Update(BattleSpriteTask* task) {
  int entry;

  if (task->frame != 0) {
    Battle_DrawSpriteCells(
        task->frame, task->unk_3c, task->unk_3e, task->unk_40,
        (s16)task->unk_48, (s16)task->unk_4a, (s16)task->unk_4c, task->unk_4e,
        task->unk_50, task->unk_54, task->unk_56, task->unk_59, task->unk_5a);
    if (task->unk_57 != 0) {
      Battle_DrawSpriteCells(
          task->frame, task->unk_42, task->unk_44, task->unk_46,
          (s16)task->unk_48, (s16)task->unk_4a, (s16)task->unk_4c, task->unk_4e,
          task->unk_50, task->unk_54, task->unk_58, task->unk_59, task->unk_5a);
    }
  }

  for (;;) {
    entry = *task->script;
    switch (entry & 0xff00) {
      case 0xff00:
        task->unk_5d = 0;
        Task_ResumeScript(task);
        return;
      case 0xfe00:
        task->unk_5c = 1;
        return;
      case 0xfd00:
        task->script = (const u16*)task->unk_30;
        task->entryFrames = 0;
        continue;
      case 0xfc00:
        switch (entry & 0xff) {
          case 0x02:
            Battle_StartSideShake(task->unk_5a);
            break;
          case 0x20:
            Battle_PlayFighterStartSe(task->unk_5a);
            break;
          case 0x21:
            Battle_PlayFighterReactSe(task->unk_5a);
            break;
          case 0x22:
            func_ov002_0235f9d8();
            break;
          case 0x23:
            Battle_PlayDefaultFighterSe(task->unk_5a);
            break;
          case 0x24:
            Battle_PlayFighterSe(task->unk_5a);
            break;
          case 0x2f:
            if (BATTLE_SIDE_UNIT_TYPE(task->unk_5a) != UNIT_OOZIUM) {
              Battle_PlayFighterSe(task->unk_5a);
            }
            break;
          case 0x25:
            if (OWN_FIGURES(task) == 5) {
              Battle_PlayFighterStartSe(task->unk_5a);
            }
            break;
          case 0x26:
            if (OWN_FIGURES(task) == 4) {
              Battle_PlayFighterStartSe(task->unk_5a);
            }
            break;
          case 0x27:
            if (OWN_FIGURES(task) == 3) {
              Battle_PlayFighterStartSe(task->unk_5a);
            }
            break;
          case 0x28:
            if (OWN_FIGURES(task) == 2) {
              Battle_PlayFighterStartSe(task->unk_5a);
            }
            break;
          case 0x29:
            if (OWN_FIGURES(task) == 1) {
              Battle_PlayFighterStartSe(task->unk_5a);
            }
            break;
          case 0x2a:
            if (OTHER_FIGURES(task) == 5) {
              Battle_PlayFighterReactSe(task->unk_5a);
            }
            break;
          case 0x2b:
            if (OTHER_FIGURES(task) == 4) {
              Battle_PlayFighterReactSe(task->unk_5a);
            }
            break;
          case 0x2c:
            if (OTHER_FIGURES(task) == 3) {
              Battle_PlayFighterReactSe(task->unk_5a);
            }
            break;
          case 0x2d:
            if (OTHER_FIGURES(task) == 2) {
              Battle_PlayFighterReactSe(task->unk_5a);
            }
            break;
          case 0x2e:
            if (OTHER_FIGURES(task) == 1) {
              Battle_PlayFighterReactSe(task->unk_5a);
            }
            break;
          default:
            task->unk_5b = entry & 0xff;
            break;
        }
        task->script++;
        continue;
      default:
        task->frame = ((const u32*)task->unk_2c)[entry >> 8];
        if (task->entryFrames == (*task->script & 0xff)) {
          task->script++;
          task->entryFrames = 0;
        }
        task->entryFrames++;
        return;
    }
  }
}
