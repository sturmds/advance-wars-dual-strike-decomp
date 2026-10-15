#include "types.h"
#include "game/menu_state.h"

/* Needed to match: the original adds the offset of motions to the
 * displacement of each load, not to the address of gMenuState. */
typedef struct MotionView {
  u8 pad_00[0x74];
  BattleMotion motion;
} MotionView;

/* TRUE when the motion's next position is left of x = -128 or outside y = -256 to 256. */
BOOL Battle_IsMotionOutOfBounds(int side, int slot) {
  MotionView* view =
      (MotionView*)((u8*)&gMenuState + side * sizeof(BattleMotion[5]) +
                    slot * sizeof(BattleMotion));

  if (view->motion.x + view->motion.speedX < -128 ||
      view->motion.y + view->motion.speedY < -256 ||
      view->motion.y + view->motion.speedY > 256) {
    return TRUE;
  }
  return FALSE;
}
