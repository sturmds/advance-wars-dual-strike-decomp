#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

BOOL Ov003Unit_TryCreate(int slot, int column, int x, int y);
void Sound_PlaySe(u32 soundId);
u16 func_0207a1dc(void);
BOOL func_ov003_0235c194(int slot, int column, int x, int y);
void func_ov003_023606ec(int slot);
void func_ov003_0236082c(int slot);
int func_ov003_0236095c(int slot);
void func_ov003_02360a88(int slot);
int Ov003_InputTouchedCircle(Ov003Input *input, int x, int y, int radius);

#define STATE data_ov003_02364928

/* Runs a slot in state OV003_SLOT_PLACING. The d-pad moves its cursor; the
 * touch button at the lower right of the screen, A or Y places a unit at the
 * cursor. With unk_1da_0 at 1 the slot then goes to OV003_SLOT_PLACED, otherwise
 * to OV003_SLOT_STATE_1. In a link game (data_02160ac0->unk_7c set), other
 * slots still placing with their cursor on the same cell move down a cell;
 * when unk_1da_0 is not 1, one that cannot move goes to OV003_SLOT_SCROLLING. */
void Ov003Slot_UpdatePlacing(int slot, Keypad *keypad, Ov003Input *input)
{
    int row = STATE->slots[slot].row;
    int i;

    if (keypad->repeat & PAD_KEY_UP) {
        func_ov003_02360a88(slot);
    }
    if (keypad->repeat & PAD_KEY_DOWN) {
        func_ov003_0236095c(slot);
    }
    if (keypad->repeat & PAD_KEY_LEFT) {
        func_ov003_0236082c(slot);
    }
    if (keypad->repeat & PAD_KEY_RIGHT) {
        func_ov003_023606ec(slot);
    }
    /* Counts down to 3 and stops there. Only a failed placement sets it to 1,
     * so the next frame tries again without a button. */
    OV003_SLOT_COOLDOWN(STATE, slot)--;
    if (OV003_SLOT_COOLDOWN(STATE, slot) == 2) {
        OV003_SLOT_COOLDOWN(STATE, slot) = 3;
    }
    if (Ov003_InputTouchedCircle(input, 240, 176, 24) != 1 && !(keypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_Y))
        && OV003_SLOT_COOLDOWN(STATE, slot) != 0) {
        return;
    }
    /* needed to match: the slot goes to the create calls as a u8 */
    if (STATE->unk_1da_0 == 1) {
        if (Ov003Unit_TryCreate((u8)slot, STATE->slots[slot].unk_00, STATE->rows[row - 1].unk_38,
                                STATE->rows[row - 1].unk_39) == 1) {
            if (slot == STATE->currentSlot) {
                Sound_PlaySe(0x10006);
            }
            if (data_02160ac0->unk_7c == 0) {
                STATE->slots[slot].unk_01_4 = OV003_SLOT_PLACED;
                return;
            }
            for (i = 0; i < 8; i++) {
                if (((func_0207a1dc() | 1) & (1 << i)) && i != slot
                    && STATE->slots[i].unk_01_4 == OV003_SLOT_PLACING
                    && STATE->slots[i].row == STATE->slots[slot].row
                    && STATE->slots[i].unk_00 == STATE->slots[slot].unk_00) {
                    func_ov003_0236095c(i);
                }
            }
            STATE->slots[slot].unk_01_4 = OV003_SLOT_PLACED;
        } else {
            OV003_SLOT_COOLDOWN(STATE, slot) = 1;
        }
    } else {
        if (func_ov003_0235c194((u8)slot, STATE->slots[slot].unk_00, STATE->rows[row - 1].unk_38,
                                STATE->rows[row - 1].unk_39) == 1) {
            if (slot == STATE->currentSlot) {
                Sound_PlaySe(0x10006);
            }
            STATE->slots[slot].unk_01_4 = OV003_SLOT_STATE_1;
            if (data_02160ac0->unk_7c != 0) {
                for (i = 0; i < 8; i++) {
                    if (((func_0207a1dc() | 1) & (1 << i)) && i != slot
                        && STATE->slots[i].unk_01_4 == OV003_SLOT_PLACING
                        && STATE->slots[i].row == STATE->slots[slot].row
                        && STATE->slots[i].unk_00 == STATE->slots[slot].unk_00) {
                        if (func_ov003_0236095c(i) == 0) {
                            STATE->slots[i].unk_01_4 = OV003_SLOT_SCROLLING;
                        }
                    }
                }
            }
        } else {
            OV003_SLOT_COOLDOWN(STATE, slot) = 1;
        }
    }
}
