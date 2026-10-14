#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

/* What Wireless_GetSharedData returns for one linked player. */
typedef struct LinkInput {
  u8 pad_00[4];
  u16 keys;
} LinkInput;

extern Keypad data_0216e4e4[];
extern Ov003Unit* data_ov003_02369534;
extern Ov003Unit data_ov003_02369538[];
extern u8 data_ov003_02365b04[];

u16 Wireless_GetChildAids(void);
LinkInput* Wireless_GetSharedData(int player);
void Keypad_UpdateState(Keypad* keypad, int keys);
void Ov003Unit_HandleInput(Ov003Unit* unit, Keypad* keypad, void* a2);
void Ov003Slot_UpdatePlacing(int slot, Keypad* keypad, void* a2);
void Ov003_ScrollWithDpad(void);

#define STATE data_ov003_02364928

/* Runs the input routine that the slot's state picks. Alone, that is the
 * current slot with the local keypad; linked, each of the 8 slots whose bit
 * is set in Wireless_GetChildAids() | 1 gets the keys received for its player. */
void Ov003_RunSlotInput(void) {
  int i;
  LinkInput* input;

  if (data_02160ac0->unk_7c == 0) {
    switch (STATE->slots[STATE->currentSlot].state) {
      case 0:
        break;
      case 1:
        Ov003Unit_HandleInput(data_ov003_02369534, gKeypad,
                              data_ov003_02365b04);
        break;
      case 2:
        Ov003Slot_UpdatePlacing(STATE->currentSlot, gKeypad,
                                data_ov003_02365b04);
        break;
      case 3:
        Ov003_ScrollWithDpad();
        break;
    }
  } else {
    for (i = 0; i < OV003_SLOT_COUNT; i++) {
      if ((Wireless_GetChildAids() | 1) & (1 << i)) {
        input = Wireless_GetSharedData((u16)i);
        Keypad_UpdateState(&data_0216e4e4[i], input->keys);
        switch (STATE->slots[i].state) {
          case 0:
            break;
          case 1:
            Ov003Unit_HandleInput(&data_ov003_02369538[i], &data_0216e4e4[i],
                                  input);
            break;
          case 2:
            Ov003Slot_UpdatePlacing(i, &data_0216e4e4[i], input);
            break;
          case 3:
            Ov003_ScrollWithDpad();
            break;
        }
      }
    }
  }
}
