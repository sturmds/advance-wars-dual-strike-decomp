#include "types.h"
#include "game/keypad.h"
#include "game/Unk_0229fb30.h"

void LinkMenu_PushSelectEntry(u8 entry);
void LinkMenu_PushCancel(void);
void LinkMenu_PushConfirm(void);
void LinkMenu_PushLower(void);
void LinkMenu_PushRaise(void);

/* Keys of player 0 on the link menu, which has five entries
 * (data_0229fb30.unk_45b is the selected one). Left and right pick the
 * previous and next entry, wrapping around, and send message 6. Up sends 7
 * and down 8 (LinkMenu_Update raises and lowers the setting). A sends 4 and
 * B sends 5. */
void LinkMenu_HandleKeys(void) {
  s8 entry;

  entry = data_0229fb30.unk_45b;
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    entry--;
    if (entry < 0) {
      entry = 4;
    }
    LinkMenu_PushSelectEntry(entry);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    entry++;
    if (entry >= 5) {
      entry = 0;
    }
    LinkMenu_PushSelectEntry(entry);
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    LinkMenu_PushRaise();
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    LinkMenu_PushLower();
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    LinkMenu_PushConfirm();
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    LinkMenu_PushCancel();
  }
}
