#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"

void Ov005_LoadCoPicture(Unk_ov005_0236bbac* state);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Update step: shows the picture of the chosen CO and goes back to step 0;
 * with no CO chosen, jumps to step 10. */
void Ov005SoundRoom_LoadPictureStep(Ov005Task* task) {
  if (data_ov005_0236bbac->coId == -1) {
    task->step = 10;
    return;
  }
  Ov005_LoadCoPicture(data_ov005_0236bbac);
  task->step = 0;
}
