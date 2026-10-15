#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/result_sprite_task.h"
#include "game/math.h"

void Task_ResumeScript(ResultSpriteTask* task);
void ResultPath_DrawAll(int lead);
void ResultRank_PlaySe(int rank);

#define PATH_FRAMES 32

/* Script step: draws the path sprites, their lead easing from 210 steps
 * behind its resting step to it over PATH_FRAMES frames. The last frame
 * plays the rank sound and resumes the script. */
void ResultSprite_PathIn(ResultSpriteTask* task) {
  task->frame++;
  ResultPath_DrawAll(195 - (5 - data_022a7a9c.rank) * 35 -
                     Math_Ease(4, 210, 0, task->frame, PATH_FRAMES));
  if (task->frame != PATH_FRAMES) {
    return;
  }
  ResultRank_PlaySe(data_022a7a9c.rank);
  task->frame = 0;
  Task_ResumeScript(task);
}
