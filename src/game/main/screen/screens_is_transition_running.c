#include "types.h"
#include "game/screen_other_map.h"
#include "game/screen.h"

int Dma_ChannelRunning(int channel);

/* TRUE while a screen transition runs on `screen`. A macro because the
 * original reads the screen pointer again after saving the state. */
#define SCREEN_TRANSITION_RUNNING(running, screen) \
  do {                                             \
    Unk_027c0280* savedGfx = data_027c0280;        \
    Unk_02080440* savedState;                      \
    Screen_SetCurrentGfx(screen);                  \
    savedState = data_027c027c;                    \
    Screen_SetCurrent(screen);                     \
    running = ScreenTransition_IsRunning();        \
    Screen_SetCurrent(savedState);                 \
    Screen_SetCurrentGfx(savedGfx);                \
  } while (0)

/* TRUE while a transition runs on either screen; otherwise polls DMA
 * channels 0 and 1 and returns FALSE. */
BOOL Screens_IsTransitionRunning(void) {
  BOOL running;

  SCREEN_TRANSITION_RUNNING(running, data_027c0288.unk_00);
  if (running == TRUE) {
    return TRUE;
  }
  SCREEN_TRANSITION_RUNNING(running, data_027c0288.unk_04);
  if (running == TRUE) {
    return TRUE;
  }
  Dma_ChannelRunning(0);
  Dma_ChannelRunning(1);
  return FALSE;
}
