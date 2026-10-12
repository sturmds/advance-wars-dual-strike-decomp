#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActingUnit_ResetCaptureIfMoved(void);
void func_020c8888(void);

/* Puts the acting unit on the tile its move ended on (actingUnitDestX, actingUnitDestY). */
void ActingUnit_CommitMove(void) {
  ActingUnit_ResetCaptureIfMoved();
  data_027c027c->unk_48[data_027c0284]->actingUnit->x =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestX;
  data_027c027c->unk_48[data_027c0284]->actingUnit->y =
      data_027c027c->unk_48[data_027c0284]->actingUnitDestY;
  func_020c8888();
}
