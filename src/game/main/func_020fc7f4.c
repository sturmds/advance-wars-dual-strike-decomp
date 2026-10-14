#include "types.h"
#include "game/link_buffers.h"
#include "sdk/mb.h"

extern int data_022a7e68;
void Gfx_ClearLayerMap(int arg);
void LargeFont_DrawName(void* dest, int a, u16 b, int c);
void Gfx_RequestLayerCopy(int layer);

void func_020fc7f4(void) {
  int saved;
  int i;
  int n;
  DlPlayPlayerBadge* entry;

  if (data_022a7e68 != 2) {
    return;
  }
  Gfx_ClearLayerMap(0);
  saved = OS_DisableInterrupts();
  n = 0;
  for (i = 1; i < LINK_AID_COUNT; i++) {
    entry = MbComm_GetChildUser(i);
    if (entry != NULL) {
      LargeFont_DrawName(entry->name, 6, 5 + n * 2, 0x200 + n * 0x28);
      n++;
    }
  }
  OS_RestoreInterrupts(saved);
  Gfx_RequestLayerCopy(0);
}
