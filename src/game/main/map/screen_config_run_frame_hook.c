#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Runs the screen config's per-frame hook with the frame count, when it has one. */
void ScreenConfig_RunFrameHook(int frame) {
  if (HOOKS->frameHook != NULL) {
    HOOKS->frameHook(frame);
  }
}
