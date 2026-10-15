#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Sound_PlaySe(u32 soundId);
BOOL Ov006_SlotsAreShown(void);
void Ov006_StartSlotsSlideOut(void);
void Ov006_ToggleChosenButton(int button);

#define STATE data_ov006_0236d640

/* Handles a press on one of the 8 editor menu buttons (ids 0x20-0x27), only
 * while Ov006_SlotsAreShown reports the buttons ready. Buttons 0x21-0x23 pick
 * view 1 (the CO preview side of Ov006_UpdateEntryView), 0x25-0x26 view 0
 * (terrain). Those toggle selection through Ov006_ToggleChosenButton (unk_4c is -1
 * after a second press) and set flags 0x10, 0x20 and 0x40 to match; 0x20,
 * 0x24 and 0x27 clear the selection state instead. Plays sound 0x10001 or
 * 0x10002. */
void Ov006_PressMenuButton(int button) {
  if (!Ov006_SlotsAreShown()) {
    return;
  }
  STATE->unk_39 = button;
  switch (button) {
    case 0x20:
      STATE->chosenButton = 0x20;
      STATE->unk_00 |= 0x10;
      STATE->unk_00 &= ~0x20;
      STATE->unk_00 &= ~0x40;
      if (Ov006_SlotsAreShown()) {
        Ov006_StartSlotsSlideOut();
      }
      Sound_PlaySe(0x10001);
      break;
    case 0x21:
      STATE->unitMode = 1;
      STATE->unk_00 |= 0x10;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      if (STATE->chosenButton == -1) {
        STATE->unk_00 &= ~0x20;
        Sound_PlaySe(0x10002);
      } else {
        STATE->unk_00 |= 0x20;
        Sound_PlaySe(0x10001);
      }
      break;
    case 0x22:
      STATE->unitMode = 1;
      STATE->unk_00 &= ~0x20;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      if (STATE->chosenButton == -1) {
        STATE->unk_00 |= 0x10;
        Sound_PlaySe(0x10002);
      } else {
        STATE->unk_00 &= ~0x10;
        Sound_PlaySe(0x10001);
      }
      break;
    case 0x23:
      STATE->unitMode = 1;
      STATE->unk_00 &= ~0x20;
      Ov006_ToggleChosenButton(button);
      if (STATE->chosenButton == -1) {
        STATE->unk_00 &= ~0x40;
        STATE->unk_00 |= 0x10;
        Sound_PlaySe(0x10002);
      } else {
        STATE->unk_00 |= 0x40;
        STATE->unk_00 &= ~0x10;
        Sound_PlaySe(0x10001);
      }
      break;
    case 0x24:
      STATE->unk_00 |= 0x10;
      STATE->unk_00 &= ~0x20;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      STATE->chosenButton = button;
      if (Ov006_SlotsAreShown()) {
        Ov006_StartSlotsSlideOut();
      }
      Sound_PlaySe(0x10002);
      break;
    case 0x25:
      STATE->unitMode = 0;
      STATE->unk_00 |= 0x10;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      if (STATE->chosenButton == -1) {
        STATE->unk_00 &= ~0x20;
        Sound_PlaySe(0x10002);
      } else {
        STATE->unk_00 |= 0x20;
        Sound_PlaySe(0x10001);
      }
      break;
    case 0x26:
      STATE->unitMode = 0;
      STATE->unk_00 &= ~0x20;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      if (STATE->chosenButton == -1) {
        STATE->unk_00 |= 0x10;
        Sound_PlaySe(0x10002);
      } else {
        STATE->unk_00 &= ~0x10;
        Sound_PlaySe(0x10001);
      }
      break;
    case 0x27:
      STATE->unk_00 |= 0x10;
      STATE->unk_00 &= ~0x20;
      STATE->unk_00 &= ~0x40;
      Ov006_ToggleChosenButton(button);
      STATE->chosenButton = button;
      if (Ov006_SlotsAreShown()) {
        Ov006_StartSlotsSlideOut();
      }
      Sound_PlaySe(0x10002);
      break;
  }
}
