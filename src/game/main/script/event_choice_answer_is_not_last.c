#include "types.h"

extern u16 gEventChoiceAnswer;

/* Returns 0 when the event prompt's answer is 1 (the last of its choices, or
 * B, see EventChoice_Update), 1 for any other answer. */
BOOL EventChoice_AnswerIsNotLast(void) {
  switch (gEventChoiceAnswer) {
    case 1:
      return FALSE;
    case 0: /* needed to match: the listed cases make the jump table */
    case 2:
    case 3:
    case 4:
    default:
      return TRUE;
  }
}
