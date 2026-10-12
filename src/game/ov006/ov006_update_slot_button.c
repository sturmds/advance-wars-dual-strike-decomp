#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_button.h"
#include "game/keypad.h"

fx32 FX_Div(fx32 numer, fx32 denom);
int Ov006_SlotsAreShown(void);
void Ov006_RemoveSlotButtons(void);
void Ov006_PressMenuButton(int buttonId);
void func_ov006_02357760(void);
void Ov006_PostRequest(int request);

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define OV6 data_ov006_0236d640

/* Per-frame update of slot buttons 32-39, which slide in from the left
 * (from x -32 to index * 32) and back out. Select, once at least 7 of them
 * are shown, calls Ov006_PressMenuButton(32). Button 39 drives the rest: when it
 * is in, it clears flag 8 and calls Ov006_PressMenuButton with 38 or 34 by
 * unitMode; once it is out it acts on chosenButton (32: request 3; 36 or 39: set
 * unitMode to 1 or 0 and request 2), then removes all eight buttons. */
void Ov006_UpdateSlotButton(MenuButton* button) {
  int index = button->id - 32;
  Ov006SlotButton* slide = &OV6->slotButtons[index];
  fx32 target;

  if (slide->state == OV006_SLIDE_NONE) {
    return;
  }
  if ((gKeypad->pressed & PAD_BUTTON_SELECT) && Ov006_SlotsAreShown()) {
    Ov006_PressMenuButton(32);
    return;
  }
  switch (slide->state) {
    case OV006_SLIDE_IN_START:
      slide->x = -32 * FX32_ONE;
      slide->speed = 0;
      slide->state = OV006_SLIDE_IN;
    case OV006_SLIDE_IN:
      /* Eases toward 16 pixels past the button's place (index * 32) and
         * stops there. */
      target = index * (32 * FX32_ONE);
      slide->x += FX_Div(target + 16 * FX32_ONE - slide->x, 6 * FX32_ONE);
      if (slide->x >= target) {
        slide->x = target;
        slide->state = OV006_SLIDE_SHOWN;
        slide->flags |= 1;
        if (button->id == 39) {
          OV6->unk_00 &= ~8;
          if (OV6->unitMode == 0) {
            Ov006_PressMenuButton(38);
          }
          if (OV6->unitMode == 1) {
            Ov006_PressMenuButton(34);
          }
        }
      }
      button->x = slide->x >> 12;
      break;
    case OV006_SLIDE_SHOWN:
      break;
    case OV006_SLIDE_OUT_START:
      slide->state = OV006_SLIDE_OUT;
      slide->flags &= ~1;
    case OV006_SLIDE_OUT:
      slide->x += FX_Div(-48 * FX32_ONE - slide->x, 5 * FX32_ONE);
      if (slide->x < -32 * FX32_ONE) {
        slide->x = -32 * FX32_ONE;
        button->flags = 0;
        if (button->id == 39) {
          slide->state = OV006_SLIDE_OUT_LAST;
        } else {
          slide->state = OV006_SLIDE_DONE;
        }
      }
      button->x = slide->x >> 12;
      break;
    case OV006_SLIDE_OUT_LAST:
      if (OV6->chosenButton == 32) {
        Ov006_PostRequest(3);
        slide->state = OV006_SLIDE_DONE;
        return;
      }
      if (OV6->chosenButton == 36 || OV6->chosenButton == 39) {
        if (OV6->chosenButton == 36) {
          OV6->unitMode = 1;
        }
        if (OV6->chosenButton == 39) {
          OV6->unitMode = 0;
        }
        func_ov006_02357760();
        slide->state = OV006_SLIDE_DONE;
      }
      break;
    case OV006_SLIDE_DONE:
      if (button->id == 39) {
        Ov006_RemoveSlotButtons();
      }
      break;
  }
}
