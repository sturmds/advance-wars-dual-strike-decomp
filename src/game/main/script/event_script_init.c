#include "types.h"
#include "game/event_script.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02088d04.h"

extern u16 gEventScriptKeys;
extern s16 gEventScriptOwnsInput;
extern u8* data_02297790;

void func_0210187c(void);
void func_0201d7a8(void);
void EventScript_ClearTextMap(void);
void EventScript_ResetLineOffsets(void);

/* Clears the event script slots and related state when a battle map starts
 * (Map_InitBattle). The last lines save the layer 0 tile data address in
 * data_02297790 when it lies below 0x6200000 (inside BG VRAM). */
void EventScript_Init(void) {
  s16 i;
  s16 j;
  u8* charBase;

  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++) {
    data_022977f0[i].start = NULL;
  }
  for (j = 0; j < EVENT_SCRIPT_QUEUE_SIZE; j++) {
    gEventScriptQueue[j] = NULL;
  }
  func_0210187c();
  gEventScriptKeys = 0;
  gEventScriptOwnsInput = 0;
  func_0201d7a8();
  EventScript_ClearTextMap();
  EventScript_ResetLineOffsets();
  data_022977a4.unk_08 = 0;
  data_022977a4.unk_00 = 0;
  data_022977a4.armyColour = 1;
  data_022977a4.unk_02 = 0;
  data_022977a4.unk_04 = 0x100;
  charBase = data_027c0280->bgVram +
             (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14);
  if ((u32)charBase < 0x6200000) {
    data_02297790 = charBase;
  }
}
