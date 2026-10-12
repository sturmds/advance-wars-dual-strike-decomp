#include "sdk/pm.h"

void PowerHook_AddFirst(PowerHookEntry** list, PowerHookEntry* info) {
  if (list != NULL) {
    info->next = *list;
    *list = info;
  }
}
