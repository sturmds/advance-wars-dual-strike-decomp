#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_QueueTerrainEntrySprites(void);
void Ov006_QueueEntrySprites(void);
void Ov006_QueueToolButtonIcons(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* With flag 0x10 set, runs Ov006_QueueEntrySprites (flag 1 clear) or
 * Ov006_QueueTerrainEntrySprites (flag 1 set), then
 * Ov006_QueueToolButtonIcons. The repeated flag 1 test and the two tails are
 * needed to match. */
void func_ov006_02351ac4(void) {
  u32 flags = data_ov006_0236d640->unk_02;

  if (!(flags & 0x10)) {
    return;
  }
  if (!(flags & 1)) {
    Ov006_QueueEntrySprites();
    Ov006_QueueToolButtonIcons();
    return;
  }
  if (flags & 1) {
    Ov006_QueueTerrainEntrySprites();
    Ov006_QueueToolButtonIcons();
  }
}
