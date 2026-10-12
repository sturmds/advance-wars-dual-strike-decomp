#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

BOOL Ov003Unit_TryCreate(int slot, int column, int x, int y);
void Sound_PlaySe(u32 soundId);
u16 Wireless_GetChildAids(void);
BOOL Ov003Unit_CreateWithSprite(int slot, int column, int x, int y);
void Ov003_MoveCursorRight(int slot);
void Ov003_MoveCursorLeft(int slot);
int Ov003_MoveCursorDown(int slot);
void Ov003_MoveCursorUp(int slot);
int Ov003_InputTouchedCircle(Ov003Input* input, int x, int y, int radius);

#define STATE data_ov003_02364928

/* Runs a slot in state OV003_SLOT_PLACING. The d-pad moves its cursor; the
 * touch button at the lower right of the screen, A or Y places a unit at the
 * cursor. With phase at 1 the slot then goes to OV003_SLOT_PLACED, otherwise
 * to OV003_SLOT_STATE_1. In a link game (data_02160ac0->unk_7c set), other
 * slots still placing with their cursor on the same cell move down a cell;
 * when phase is not 1, one that cannot move goes to OV003_SLOT_SCROLLING. */
void Ov003Slot_UpdatePlacing(int slot, Keypad* keypad, Ov003Input* input) {
  int row = STATE->slots[slot].row;
  int i;

  if (keypad->repeat & PAD_KEY_UP) {
    Ov003_MoveCursorUp(slot);
  }
  if (keypad->repeat & PAD_KEY_DOWN) {
    Ov003_MoveCursorDown(slot);
  }
  if (keypad->repeat & PAD_KEY_LEFT) {
    Ov003_MoveCursorLeft(slot);
  }
  if (keypad->repeat & PAD_KEY_RIGHT) {
    Ov003_MoveCursorRight(slot);
  }
  /* Counts down to 3 and stops there. Only a failed placement sets it to 1,
     * so the next frame tries again without a button. */
  OV003_SLOT_COOLDOWN(STATE, slot)--;
  if (OV003_SLOT_COOLDOWN(STATE, slot) == 2) {
    OV003_SLOT_COOLDOWN(STATE, slot) = 3;
  }
  if (Ov003_InputTouchedCircle(input, 240, 176, 24) != 1 &&
      !(keypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_Y)) &&
      OV003_SLOT_COOLDOWN(STATE, slot) != 0) {
    return;
  }
  /* needed to match: the slot goes to the create calls as a u8 */
  if (STATE->phase == 1) {
    if (Ov003Unit_TryCreate((u8)slot, STATE->slots[slot].cursor,
                            STATE->rows[row - 1].unk_38,
                            STATE->rows[row - 1].unk_39) == 1) {
      if (slot == STATE->currentSlot) {
        Sound_PlaySe(0x10006);
      }
      if (data_02160ac0->unk_7c == 0) {
        STATE->slots[slot].state = OV003_SLOT_PLACED;
        return;
      }
      for (i = 0; i < 8; i++) {
        if (((Wireless_GetChildAids() | 1) & (1 << i)) && i != slot &&
            STATE->slots[i].state == OV003_SLOT_PLACING &&
            STATE->slots[i].row == STATE->slots[slot].row &&
            STATE->slots[i].cursor == STATE->slots[slot].cursor) {
          Ov003_MoveCursorDown(i);
        }
      }
      STATE->slots[slot].state = OV003_SLOT_PLACED;
    } else {
      OV003_SLOT_COOLDOWN(STATE, slot) = 1;
    }
  } else {
    if (Ov003Unit_CreateWithSprite((u8)slot, STATE->slots[slot].cursor,
                                   STATE->rows[row - 1].unk_38,
                                   STATE->rows[row - 1].unk_39) == 1) {
      if (slot == STATE->currentSlot) {
        Sound_PlaySe(0x10006);
      }
      STATE->slots[slot].state = OV003_SLOT_STATE_1;
      if (data_02160ac0->unk_7c != 0) {
        for (i = 0; i < 8; i++) {
          if (((Wireless_GetChildAids() | 1) & (1 << i)) && i != slot &&
              STATE->slots[i].state == OV003_SLOT_PLACING &&
              STATE->slots[i].row == STATE->slots[slot].row &&
              STATE->slots[i].cursor == STATE->slots[slot].cursor) {
            if (Ov003_MoveCursorDown(i) == 0) {
              STATE->slots[i].state = OV003_SLOT_SCROLLING;
            }
          }
        }
      }
    } else {
      OV003_SLOT_COOLDOWN(STATE, slot) = 1;
    }
  }
}
