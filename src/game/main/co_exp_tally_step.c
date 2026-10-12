#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/win_lose.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0229c734.h"

int Campaign_IsHardMode(void);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(void* task);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);
int CoExp_IsSingleColumn(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern MenuState gMenuState;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define EXP gMenuState.coExp.exp

/* Task step of the results screen: takes up to 2 points off the pool
 * (ResultPoint_GetTally) each frame and adds them to the front and back CO of
 * each player, scaled by game mode: War Room by unk_59 (0: x2.5, 1: x2,
 * 2: x1.5; only the front CO when CoExp_IsSingleColumn says so), hard campaign
 * x2. A player with one CO gets both shares.
 * A or a touch hands out the rest at once. Resumes the script once the
 * pool is empty. */
void CoExp_TallyStep(WinLoseTask* task) {
  int step;
  int i;
  int k;
  int gain;

  ResultPoint_SetTally((s16)(ResultPoint_GetTally() - 2));
  step = 2;
  while (ResultPoint_GetTally() < 0) {
    ResultPoint_SetTally((s16)(ResultPoint_GetTally() + 1));
    step--;
  }

  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i >= 3) {
      break;
    }
    /* needed to match: (step << 1) and gain <<= 1 rather than step * 2
         * where the original computes the x2 share apart */
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
      if (CoExp_IsSingleColumn() == 1) {
        EXP[i * 2].points +=
            STATE->unk_59 == 0 ? (step << 1) + (step >> 1) : step * 2;
      } else {
        if (STATE->unk_59 == 0) {
          gain = (step << 1) + (step >> 1);
        } else if (STATE->unk_59 == 1) {
          gain = step * 2;
        } else if (STATE->unk_59 == 2) {
          gain = step + (step >> 1);
        } else {
          gain = step;
        }
        EXP[i * 2].points += gain;
        EXP[i * 2 + 1].points += gain;
      }
    } else {
      gain = step;
      if (STATE->gameMode == GAME_MODE_CAMPAIGN && Campaign_IsHardMode() == 1) {
        gain <<= 1;
      }
      EXP[i * 2].points += gain;
      if (EXP[i * 2 + 1].level < 0) {
        EXP[i * 2].points += gain;
      } else {
        EXP[i * 2 + 1].points += gain;
      }
    }
    for (k = 0; k < 2; k++) {
      if (EXP[i * 2 + k].points > CO_EXP_PER_LEVEL - 1) {
        EXP[i * 2 + k].level++;
        EXP[i * 2 + k].points -= CO_EXP_PER_LEVEL;
        if (EXP[i * 2 + k].level <= CO_EXP_MAX_LEVEL) {
          EXP[i * 2 + k].levelUpFrame = 1;
        }
      }
      if (EXP[i * 2 + k].level >= CO_EXP_MAX_LEVEL) {
        EXP[i * 2 + k].level = CO_EXP_MAX_LEVEL;
        EXP[i * 2 + k].points = 0;
      }
    }
  }

  if ((task->frame & 1) == 0) {
    Sound_PlaySe(0x1000c); /* a tick every other frame */
  }
  /* needed to match: the original adds the offset to gMenuState at run time */
  CoExp_DrawScreen(
      (CoExp*)((u8*)&gMenuState + (u32) & ((MenuState*)0)->coExp.exp), 0, 0,
      -1);
  if (ResultPoint_GetTally() == 0) {
    task->frame = 0;
    Task_ResumeScript(task);
    return;
  }
  task->frame++;
  if (!(gKeypad->pressed & PAD_BUTTON_A)) {
    if (gTouch.phase != TOUCH_PRESSED || gTouch.active != 1) {
      return;
    }
  }

  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i >= 3) {
      break;
    }
    step = ResultPoint_GetTally();
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
      if (CoExp_IsSingleColumn() == 1) {
        EXP[i * 2].points +=
            STATE->unk_59 == 0 ? (step << 1) + (step >> 1) : step * 2;
      } else {
        if (STATE->unk_59 == 0) {
          step = (step << 1) + (step >> 1);
        } else if (STATE->unk_59 == 1) {
          step = step * 2;
        } else if (STATE->unk_59 == 2) {
          step = step + (step >> 1);
        }
        EXP[i * 2].points += step;
        EXP[i * 2 + 1].points += step;
      }
    } else {
      if (STATE->gameMode == GAME_MODE_CAMPAIGN && Campaign_IsHardMode() == 1) {
        step = step * 2;
      }
      EXP[i * 2].points += step;
      if (EXP[i * 2 + 1].level < 0) {
        EXP[i * 2].points += step;
      } else {
        EXP[i * 2 + 1].points += step;
      }
    }
    for (k = 0; k < 2; k++) {
      while (EXP[i * 2 + k].points > CO_EXP_PER_LEVEL - 1) {
        EXP[i * 2 + k].level++;
        EXP[i * 2 + k].points -= CO_EXP_PER_LEVEL;
        if (EXP[i * 2 + k].level <= CO_EXP_MAX_LEVEL) {
          EXP[i * 2 + k].levelUpFrame = 1;
        }
      }
      if (EXP[i * 2 + k].level >= CO_EXP_MAX_LEVEL) {
        EXP[i * 2 + k].level = CO_EXP_MAX_LEVEL;
        EXP[i * 2 + k].points = 0;
      }
    }
  }
  ResultPoint_SetTally(0);
  task->frame = 0;
  Task_ResumeScript(task);
}
