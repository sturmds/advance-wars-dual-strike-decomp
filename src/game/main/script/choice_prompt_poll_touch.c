#include "types.h"
#include "game/touch.h"

u32 ChoicePrompt_FindTouchedOption(int x, int y);

/* Option touched at the start of the current touch. */
extern int gChoicePromptTouchedOption;

/* Key bits the touch stands in for, as the caller reads them. */
#define CHOICE_TOUCH_LEFT 0x04
#define CHOICE_TOUCH_RIGHT 0x08
#define CHOICE_TOUCH_CONFIRM 0x10

/* Turns a touch on the two choice prompt options into key bits: touching
 * option 1 or 2 moves the cursor left or right, and ending the touch on
 * the option where it started confirms. */
int ChoicePrompt_PollTouch(void) {
  int option = ChoicePrompt_FindTouchedOption(gTouch.x, gTouch.y);
  int phase = gTouch.phase;

  switch (phase) {
    case TOUCH_HELD: /* needed to match, though default covers it */
    default:
      if (Touch_InPhase(phase, TOUCH_HELD)) {
        gChoicePromptTouchedOption = 0;
      }
      /* fall through */
    case TOUCH_PRESSED:
      if (Touch_InPhase(phase, TOUCH_PRESSED)) {
        gChoicePromptTouchedOption = option;
      } else if (gChoicePromptTouchedOption != option) {
        gChoicePromptTouchedOption = 0;
      }
      switch (option) {
        case 1:
          return CHOICE_TOUCH_LEFT;
        case 2:
          return CHOICE_TOUCH_RIGHT;
        default:
          return 0;
      }
    case TOUCH_RELEASED:
      if (Touch_InPhase(phase, TOUCH_RELEASED) &&
          gChoicePromptTouchedOption == option) {
        switch (gChoicePromptTouchedOption) {
          case 1:
            return CHOICE_TOUCH_CONFIRM;
          case 2:
            return CHOICE_TOUCH_CONFIRM;
          default:
            return 0;
        }
      }
      break;
  }
  return 0;
}
