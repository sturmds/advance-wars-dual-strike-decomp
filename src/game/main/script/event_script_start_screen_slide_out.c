#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02088d04.h"
#include "game/event_script.h"

extern const char data_02158734[];
extern u16 data_022978e0[];
extern s16 data_02297780;
extern EventSlotHold data_022977f8[];
extern EventSlotCounter data_022977fe[];
extern Unk_027c0280* data_027c0280;

void* File_Load(const char* path);
void Mem_Free(void* res);
void Bg_PutTileBlock(void* target, int a, int b, const void* src, int c);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void EventScript_ClearTextMap(void);
void EventPicture_Show(u16 coMood);
void CallbackList_Add(void* callback);
void func_0201dc48(void);
void EventHold_StepScreenSlideOut(EventScriptSlot* slot);

/* Called by EventScript_ShowPicture, the helper of EventCmd_ShowPicture. Loads the file data_02158734, puts its tile block (palette
 * from data_022977a4.unk_00) in the table data_022978e0 and queues the copy to VRAM, clears the
 * text map, loads the CO picture that data_022977a4.unk_02 names, and starts the slide-out
 * hold for the slot at counter 47. */
void EventScript_StartScreenSlideOut(int index) {
  u8* file = File_Load(data_02158734);
  u16 palette = data_022977a4.unk_00 << 12;

  Bg_PutTileBlock(data_022978e0, 0, 0, file + 0x1da, palette);
  Mem_Free(file);
  TransferQueue_Add(data_022978e0, data_027c0280->bgVram + 0x1f800, 0x180);
  EventScript_ClearTextMap();
  EventPicture_Show(data_022977a4.unk_02);
  data_022977fe[index].counter = 47;
  data_02297780 = data_022977fe[index].counter;
  CallbackList_Add(func_0201dc48);
  data_022977f8[index].hold = (u32)EventHold_StepScreenSlideOut;
}
