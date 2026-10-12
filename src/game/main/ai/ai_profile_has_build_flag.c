#include "types.h"
#include "game/ai_profile.h"

/* TRUE when the profile's build flag byte has any bit of mask set (masks used: 1
 * Piperunners, 4 Black Boats, 8 Landers). */
BOOL AiProfile_HasBuildFlag(int mask) {
  return (gAiProfile->unk_08[9] & mask) != 0;
}
