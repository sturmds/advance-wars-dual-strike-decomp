#include "sdk/pm.h"

void PowerHook_Unlink(PowerHookEntry** list, PowerHookEntry* info) {
  PowerHookEntry* cur;
  PowerHookEntry* prev;

  if (list == NULL) {
    return;
  }
  prev = cur = *list;
  while (cur != NULL) {
    if (cur == info) {
      if (cur == prev) {
        *list = cur->next;
      } else {
        prev->next = cur->next;
      }
      return;
    }
    prev = cur;
    cur = cur->next;
  }
}
