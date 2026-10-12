#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Ov005_LoadCoPicture(Unk_ov005_0236bbac* state);
void Ov005SoundRoom_QueueSprite(Unk_ov005_0236bbac* state, int x, int y);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Swaps the CO picture when state->coId changes: the picture slides
 * 144 pixels right over 8 frames, the new CO is loaded, 4 frames pass, and
 * it slides back. While the CO stays the same the picture rests at
 * (pictureX, pictureY). Step 10 waits for a change without moving. */
void Ov005_UpdateCoPictureSwap(Ov005Task* task) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  int offset;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = 8;
      task->step++;
    case 1:
      if (state->coId != state->loadedCoId) {
        task->frame++;
      } else {
        task->frame--;
      }
      if (task->frame < 0) {
        task->frame = 0;
      }
      offset = Math_Ease(1, 0, 0x90000, task->frame, task->frames);
      if (task->frame >= task->frames) {
        task->step++;
      }
      Ov005SoundRoom_QueueSprite(state, (state->pictureX + offset) >> 12,
                                 state->pictureY >> 12);
      break;
    case 2:
      Ov005_LoadCoPicture(state);
      state->loadedCoId = state->coId;
      task->frame = 0;
      task->frames = 4;
      task->step++;
    case 3:
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 4:
      task->frame = 8;
      task->frames = 8;
      task->step = 1;
      break;
    case 10:
      if (state->coId != state->loadedCoId) {
        task->step = 2;
      }
      break;
  }
}
