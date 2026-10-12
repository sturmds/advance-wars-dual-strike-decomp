#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

Ov004Resource* Ov004_FindResource(Unk_ov004_022bf158* state, const char* name);

void* Ov004_GetResourceData(const char* name) {
  Ov004Resource* resource = Ov004_FindResource(data_ov004_022bf158, name);

  if (resource != NULL) {
    return resource->data;
  }
  return NULL;
}
