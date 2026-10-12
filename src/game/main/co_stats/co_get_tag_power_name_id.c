#include "types.h"
#include "game/co_data.h"

const CoTagRecord* Co_GetTagRecord(int co, int partner);

/* Text id of the tag power name of a CO pair; a default text (group 0xf000,
 * index 0x1e7) for a pair with no record. */
u32 Co_GetTagPowerNameId(int co, int partner) {
  const CoTagRecord* record = Co_GetTagRecord(co, partner);

  if (record == NULL) {
    return 0xf00001e7;
  }
  return record->tagPowerName.id;
}
