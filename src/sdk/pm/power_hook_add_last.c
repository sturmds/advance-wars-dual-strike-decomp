#include "sdk/pm.h"

void PowerHook_AddLast(PowerHookEntry **list, PowerHookEntry *info)
{
    PowerHookEntry *last;
    PowerHookEntry *next;

    if (list == NULL) {
        return;
    }
    last = *list;
    if (last == NULL) {
        info->next = NULL;
        *list = info;
        return;
    }
    next = last->next;
    while (next != NULL) {
        last = next;
        next = next->next;
    }
    info->next = next;
    last->next = info;
}
