#include "types.h"
#include "game/rect.h"

extern Rect data_02297794;
extern Rect data_0229779c;

/* Returns 1 when the touch point is in the choice prompt's first option
 * rectangle, 2 for the second, 0 for neither. */
u32 ChoicePrompt_FindTouchedOption(int x, int y) {
  if (Rect_ContainsPoint(x, y, &data_02297794)) {
    return 1;
  }
  if (Rect_ContainsPoint(x, y, &data_0229779c)) {
    return 2;
  }
  return 0;
}
