#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/event_script.h"
#include "game/rect.h"

s16 gEventTextWindowPos;
s16 data_02297788;
u8* data_02297790; /* layer 0 tile data in main BG VRAM */
Rect data_0229779c;
Rect data_02297794;
Unk_022977a4 data_022977a4;
const EventScriptCommand* gEventScriptQueue[EVENT_SCRIPT_QUEUE_SIZE];
Rect gEventChoiceButtons[4]; /* touch buttons of the four choices */
EventScriptSlot data_022977f0[EVENT_SCRIPT_SLOTS];
u8 data_02297a60[0x180]; /* 0x180 bytes no code names, before gEventTextMap */
u16 data_022978e0[192];
u16 gEventTextMap[0x180]; /* size: up to the next symbol */
extern s16 data_02297780;

void* func_0201dc48(void) {
  data_02297788 = data_02297780;
  return &data_02297788;
}
