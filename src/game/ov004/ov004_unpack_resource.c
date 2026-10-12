#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

Ov004Resource* Ov004_FindResource(Unk_ov004_022bf158* state, const char* name);
void Data_Unpack(const void* src, void* dst);

void Ov004_UnpackResource(const char* name, void* dst) {
  Ov004Resource* resource = Ov004_FindResource(data_ov004_022bf158, name);

  if (resource == NULL) {
    return;
  }
  Data_Unpack(resource->data, dst);
}
