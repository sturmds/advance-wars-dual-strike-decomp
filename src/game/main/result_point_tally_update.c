#include "types.h"
#include "game/Unk_0229c734.h"
#include "game/math.h"

void ResultPoint_DrawPanel(int x, int y, int arg);
void ResultPoint_DrawPanelTally(int x, int y, ResultPointTask* task);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(ResultPointTask* task);
void ResultPoint_DrawRows(int x, int y, int arg);
void ResultPoint_DrawBar(int x, int y);
int ScoreList_CountRowsAtLeast(int points);
int ScoreList_GetPanelSize(int points);
void ScoreList_NextPage(void);
BOOL ScoreList_HasMoreRows(void);

#define SCORES data_0229c734

/* One count frame: each row of the page whose points reach countStep adds
 * one point to the tally. When countStep reaches pageMaxPoints plus
 * task->rank, starts the next page if there is one, else resumes the
 * script. A macro because the original has this code twice. */
#define COUNT_STEP(task, done)                                        \
  do {                                                                \
    int add;                                                          \
    if ((task)->countStep != 0 &&                                     \
        (add = ScoreList_CountRowsAtLeast((task)->countStep)) != 0) { \
      SCORES->tally += add;                                           \
      if (((task)->frame & 1) == 0) {                                 \
        Sound_PlaySe(0x1000c);                                        \
      }                                                               \
    }                                                                 \
    ResultPoint_DrawRows(8, 16, (task)->countStep);                   \
    if ((task)->countStep == SCORES->pageMaxPoints + (task)->rank) {  \
      (task)->frame = 0;                                              \
      if (ScoreList_HasMoreRows() == TRUE) {                          \
        SCORES->pageIndex++;                                          \
      } else {                                                        \
        Task_ResumeScript(task);                                      \
        SCORES->unk_a4 = 0;                                           \
      }                                                               \
      goto done;                                                      \
    }                                                                 \
    (task)->countStep++;                                              \
  } while (0)

/* Task update of the score list: counts the rows' points into the tally,
 * page by page. Between pages the rows slide out to the left over 24
 * frames, then the next page slides in over 24 frames. The panel below
 * shows the tally, and the tally version of it once all points are in. */
void ResultPoint_TallyUpdate(ResultPointTask* task) {
  if (SCORES->pageIndex == 0) {
    COUNT_STEP(task, draw);
  } else {
    if (task->frame == 24) {
      task->countStep = 0;
      ScoreList_NextPage();
      task->rank = ScoreList_GetPanelSize(task->totalPoints);
    }
    if (task->frame < 24) {
      ResultPoint_DrawRows(Math_Ease(1, 8, -160, task->frame, 24), 16,
                           task->countStep);
    } else if (task->frame < 48) {
      ResultPoint_DrawRows(Math_Ease(4, -160, 8, task->frame - 24, 24), 16, -1);
    } else {
      COUNT_STEP(task, draw);
    }
  }
  task->frame++;
draw:
  ResultPoint_DrawBar(8, 160);
  if (SCORES->tally == task->totalPoints && ScoreList_HasMoreRows() == FALSE) {
    ResultPoint_DrawPanelTally(8, 160, task);
    task->tallyPopFrame++;
  } else {
    ResultPoint_DrawPanel(8, 160, SCORES->tally);
  }
}
