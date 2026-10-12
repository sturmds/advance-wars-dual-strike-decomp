#include "types.h"
#include "game/unlocks.h"

void CoSelect_ClearAvailable(void);
void CoSelect_AddAvailable(u8 coId);

extern u8 data_021687c4[];

/* Clears the available-CO mask, then adds each CO of the zero-terminated list
 * data_021687c4 whose colour flag is set (CoColor_IsSet). Needed to match: do-while
 * loop, u32 co. */
void CoSelect_ResetToUnlockedCos(void) {
  u32 co;
  u8* list;

  CoSelect_ClearAvailable();
  list = data_021687c4;
  if (*list == 0) {
    return;
  }
  do {
    co = *list;
    if (CoColor_IsSet(co)) {
      CoSelect_AddAvailable(co);
    }
  } while (*++list != 0);
}
