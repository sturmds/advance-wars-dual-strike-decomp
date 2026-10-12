#include "types.h"

extern u8 data_02290720;
extern u8 data_02290721;
extern u8 data_02290722;
extern u8 data_02290723;
extern u8 data_02290724;
extern u8 data_02290725;

/* Returns the address of the one-byte value for index 1 to 6, or NULL. Callers
 * BattleSave_SetRecordFlag and func_020193bc store and load it; what it counts is unknown. */
u8* func_020193e0(int index) {
  switch (index) {
    case 2:
      return &data_02290720;
    case 3:
      return &data_02290721;
    case 4:
      return &data_02290722;
    case 5:
      return &data_02290723;
    case 6:
      return &data_02290724;
    case 1:
      return &data_02290725;
    default:
      return NULL;
  }
}
