#include "types.h"
#include "sdk/card.h"

/* Card removal check by interrupt: the card line interrupt flag means the card is gone. */
BOOL Card_TestRemovalIrqFlag(void)
{
    BOOL present = TRUE;

    if (*REG_IF & IRQ_CARD_IREQ) {
        present = FALSE;
        gCardRemoved = TRUE;
    }
    return present;
}
