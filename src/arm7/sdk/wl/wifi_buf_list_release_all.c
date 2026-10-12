#include "sdk/wl.h"

/* Returns buffer->next; its definition still says int. */
int func_arm7_037f8a5c(WlBuffer* buffer);

/* Frees every buffer in the list. */
void WifiBufList_ReleaseAll(WlBufferList* list) {
  WlBuffer* buffer = list->first;
  WlBuffer* next;

  if (list->count != 0) {
    while (buffer != WIFI_LIST_END) {
      next = (WlBuffer*)func_arm7_037f8a5c(buffer);
      WifiBuffer_Release(list, buffer);
      buffer = next;
    }
  }
}
