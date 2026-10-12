#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
void SubScreen_QueueTwoSprites(int x);
void TotalRankScreen_QueueTopCoSprites(int x);
void CampaignRecords_GetAverageScore(void);
void Score_ToRank(void);
void ResultRank_PlaySe(void);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task update: over 32 frames, eases the sub screen's layers 0 and 1 in from
 * 192 pixels to the right (BG x scroll 192 to 0), and passes the offset to
 * TotalRankScreen_QueueTopCoSprites (negated) and SubScreen_QueueTwoSprites. On the last frame it runs three
 * helpers and resumes the task's script. */
void SubScreenLayers01_SlideInUpdate(Task* task) {
  Unk_02080440* saved027c;
  u8 offset;
  int layer1;
  int layer0;
  Unk_027c0280* saved0280;

  FRAME(task)++;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  offset = Math_Ease(4, 192, 0, FRAME(task), 32);
  layer1 = data_027c0280->layer1Bg;
  layer0 = data_027c0280->layer0Bg;
  data_027c0280->bgScrollX[layer1] = offset;
  data_027c0280->bgScrollX[layer0] = data_027c0280->bgScrollX[layer1];
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  TotalRankScreen_QueueTopCoSprites(-offset);
  SubScreen_QueueTwoSprites(offset);

  if (FRAME(task) != 32) {
    return;
  }
  CampaignRecords_GetAverageScore();
  Score_ToRank();
  ResultRank_PlaySe();
  FRAME(task) = 0;
  Task_ResumeScript(task);
}
