#include "types.h"
#include "game/missile_strike_task.h"

void func_020f5888(int x, int y);
void ScreenConfig_RunActionHook(void);

/* Script step: scrolls the map view to the strike tile (func_020f5888 ends in
 * the scroll and script-slot routine MapScroll_Start), then runs the screen
 * action hook. */
void MissileStrike_ScrollToTarget(MissileStrikeTask* task) {
  func_020f5888((s16)task->tileX, (s16)task->tileY);
  ScreenConfig_RunActionHook();
}
