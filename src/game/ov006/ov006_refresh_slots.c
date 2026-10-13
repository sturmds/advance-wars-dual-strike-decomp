#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_LoadSlotTiles(void);
void Ov006_QueueSlotSprites(void);

/* also declared in src/game/ov006/design_room_exit.c and 105 more */
Unk_ov006_0235f690* data_ov006_0236d640;

/* Handles request 5 in DesignRoom_Update. When a request was just posted
 * (flag 0x8000), clears the flag and screenState and loads the slot tiles. While
 * screenState is 0, queues the slot sprites. */
void Ov006_RefreshSlots(void) {
  if (data_ov006_0236d640->unk_00 & 0x8000) {
    data_ov006_0236d640->unk_00 &= ~0x8000;
    data_ov006_0236d640->screenState = 0;
    Ov006_LoadSlotTiles();
  }
  if (data_ov006_0236d640->screenState == 0) {
    Ov006_QueueSlotSprites();
  }
}
