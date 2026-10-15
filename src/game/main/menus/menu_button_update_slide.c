#include "types.h"
#include "game/menu_button.h"
#include "game/math.h"

int MenuButton_TestFlags(MenuButton* entry, int mask);

/* Moves a button that slides onto or off the screen, once per frame.
 * Sliding in starts past an edge of the 256x192 screen and ends at
 * slideToX/Y, set before the slide starts. Sliding out starts at the
 * current position. With no slide flag set the move ends on the next
 * frame. Math_Ease computes each step (curve 5 in, curve 2 out). */
void MenuButton_UpdateSlide(MenuButton* button) {
  switch (button->slideState) {
    case MENU_BUTTON_SLIDE_IN_START:
      button->slideState = MENU_BUTTON_SLIDING_IN;
      button->slideTimer = 0;
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_IN_LEFT) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = -button->width;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_IN_RIGHT) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = 256;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_IN_TOP) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromY = -button->height;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_IN_BOTTOM) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromY = 192;
      }
      if (button->slideTimer == 0) {
        return;
      }
      break;
    case MENU_BUTTON_SLIDE_OUT_START:
      button->slideState = MENU_BUTTON_SLIDING_OUT;
      button->slideTimer = 0;
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_OUT_LEFT) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = button->x;
        button->slideFromY = button->y;
        button->slideToX = -button->width;
        button->slideToY = button->y;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_OUT_RIGHT) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = button->x;
        button->slideFromY = button->y;
        button->slideToX = 256;
        button->slideToY = button->y;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_OUT_TOP) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = button->x;
        button->slideFromY = button->y;
        button->slideToX = button->x;
        button->slideToY = -button->height;
      }
      if (MenuButton_TestFlags(button, MENU_BUTTON_SLIDE_OUT_BOTTOM) != 0) {
        button->slideTimer = button->slideDuration;
        button->slideFromX = button->x;
        button->slideFromY = button->y;
        button->slideToX = button->x;
        button->slideToY = 192;
      }
      if (button->slideTimer == 0) {
        return;
      }
      break;
  }

  switch (button->slideState) {
    case MENU_BUTTON_SLIDING_IN:
      if (button->slideTimer <= 0) {
        button->slideTimer = 0;
        button->slideState = MENU_BUTTON_SHOWN;
      } else {
        button->slideTimer--;
      }
      button->x = Math_Ease(5, button->slideFromX, button->slideToX,
                            button->slideDuration - button->slideTimer,
                            button->slideDuration);
      button->y = Math_Ease(5, button->slideFromY, button->slideToY,
                            button->slideDuration - button->slideTimer,
                            button->slideDuration);
      break;
    case MENU_BUTTON_SLIDING_OUT:
      if (button->slideTimer <= 0) {
        button->slideTimer = 0;
        button->slideState = MENU_BUTTON_HIDDEN;
        if (button->onSlideOutDone != NULL) {
          button->onSlideOutDone(button);
        }
      } else {
        button->slideTimer--;
      }
      button->x = Math_Ease(2, button->slideFromX, button->slideToX,
                            button->slideDuration - button->slideTimer,
                            button->slideDuration);
      button->y = Math_Ease(2, button->slideFromY, button->slideToY,
                            button->slideDuration - button->slideTimer,
                            button->slideDuration);
      break;
  }
}
