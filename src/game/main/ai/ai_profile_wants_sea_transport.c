#include "types.h"
#include "game/ai_profile.h"

/* Bits 2 and 3 of the profile's byte 9; the sea transport code reads them
 * to decide whether to look for a transport. */
BOOL AiProfile_WantsSeaTransport(void) {
  return (gAiProfile->unk_08[9] & 0xc) != 0;
}
