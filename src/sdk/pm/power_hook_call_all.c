#include "sdk/pm.h"

void PowerHook_CallAll(PowerHookEntry* list) {
  PowerHookEntry* info;

  if (list == NULL) {
    return;
  }
  info = list;
  do {
    info->callback(info->arg);
    info = info->next;
  } while (info != NULL);
}
