#include "types.h"
#include "game/Unk_021694c0.h"

typedef struct ShakeOffsets {
  s8 xy[8][2];
} ShakeOffsets;

extern Unk_021694c0* data_021694c0;
extern ShakeOffsets gEffectSceneShakeOffsets;

/* Script step of the effect scene: shakes the scene by setting unk_2c and
 * unk_30 (added to the sprite positions) to the next x, y pair of the table.
 * The table ends with the pair (0, 0), after which the shake starts over. */
void EffectScene_ShakeStep(Unk_021694c0Task* task) {
  ShakeOffsets offsets = gEffectSceneShakeOffsets;

  data_021694c0->unk_2c = offsets.xy[task->frame][0];
  data_021694c0->unk_30 = offsets.xy[task->frame][1];
  task->frame++;
  if (offsets.xy[task->frame][0] == 0 && offsets.xy[task->frame][1] == 0) {
    task->frame = 0;
  }
}
