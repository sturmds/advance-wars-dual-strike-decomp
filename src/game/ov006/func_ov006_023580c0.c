#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int DesignMap_CopyName(int id, u8* buf);

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* 0 if any of the three design map slots holds a map, else 2. */
int func_ov006_023580c0(void) {
  if (DesignMap_CopyName(0, &data_ov006_0236d640->unk_eb) == 1) {
    return 0;
  }
  if (DesignMap_CopyName(1, &data_ov006_0236d640->unk_eb) == 1) {
    return 0;
  }
  if (DesignMap_CopyName(2, &data_ov006_0236d640->unk_eb) == 1) {
    return 0;
  }
  return 2;
}
