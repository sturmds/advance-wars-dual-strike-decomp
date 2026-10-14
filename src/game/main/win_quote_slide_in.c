#include "types.h"
#include "game/task.h"
#include "game/win_quote.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
void WinQuote_Print(int record);

extern Unk_027c0280* data_027c0280;

#define SLIDE_FRAMES 16

/* Step of the win quote screen: over 16 frames the COs slide in from the
 * sides and their pictures and layer 1 slide up 168 pixels. Then it prints
 * the quote and moves on. */
void WinQuote_SlideIn(Task* task) {
  int slideX;
  int slideY;

  WIN_QUOTE_FRAME(task)++;
  slideX = Math_Ease(4, 256, 0, WIN_QUOTE_FRAME(task), SLIDE_FRAMES);
  slideY = Math_Ease(4, 168, 0, WIN_QUOTE_FRAME(task), SLIDE_FRAMES);

  if (data_022a7aba[WIN_QUOTE_RECORD(task)].unk_00 & CO_ID_MASK) {
    Sprite_QueueCoPortrait((slideX + 64) & 0x1ff, 160, 0x5800, 0x224, 5, FALSE);
    Sprite_QueueCoPortrait((192 - slideX) & 0x1ff, 160, 0x6800, 0x3d4, 6, TRUE);
    if (slideY + 88 <= 240) {
      Sprite_QueueDefaultTile(96, (slideY + 88) & 0xff, 0x4000, 0x1dc, 6);
    }
    if (slideY + 128 <= 240) {
      Sprite_QueueDefaultTile(96, (slideY + 128) & 0xff, 0x4000, 0x1e8, 6);
    }
  } else {
    Sprite_QueueCoPortrait((slideX + 64) & 0x1ff, 160, 0x5800, 0x224, 5, FALSE);
    if (slideY + 128 <= 240) {
      Sprite_QueueDefaultTile(96, (slideY + 128) & 0xff, 0x4000, 0x1dc, 6);
    }
  }
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = -slideY;

  if (WIN_QUOTE_FRAME(task) == SLIDE_FRAMES) {
    WIN_QUOTE_FRAME(task) = 0;
    WinQuote_Print(WIN_QUOTE_RECORD(task));
    Task_ResumeScript(task);
  }
}
