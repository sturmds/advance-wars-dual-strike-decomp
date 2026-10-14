#include "types.h"
#include "game/Unk_ov004_022bf158.h"

int strcmp(const char* a, const char* b); /* strcmp */

/* Finds a resource by name. A key at or above 0x10000 is a string pointer and
 * is also compared as text; a smaller key is a plain id compared by value. */
Ov004Resource* Ov004_FindResource(Unk_ov004_022bf158* state, const char* name) {
  u32 i;
  Ov004Resource* resource = state->resources;

  for (i = 0; i < ARRAY_COUNT(state->resources); i++, resource++) {
    BOOL found = FALSE;

    if (((u32)name & 0xffff0000) != 0 &&
        ((u32)resource->name & 0xffff0000) != 0) {
      if (strcmp(name, resource->name) == 0) {
        found = TRUE;
      }
    }
    if (name == resource->name) {
      found = TRUE;
    }
    if (found) {
      return resource;
    }
  }
  return NULL;
}
