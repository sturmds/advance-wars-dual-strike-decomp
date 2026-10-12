#include "types.h"

extern u16 gEventChoiceAnswer;

/* Campaign save slot for the last event choice answer: answers 2, 3 and 4
 * give slots 0, 1 and 2; any other answer gives slot 0. */
int EventChoice_GetCampaignSlot(void) {
  switch (gEventChoiceAnswer) {
    case 2:
      return 0;
    case 3:
      return 1;
    case 4:
      return 2;
    default:
      return 0;
  }
}
