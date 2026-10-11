#include "types.h"

extern u16 data_0216e2a4;
extern u16 data_0216e2ac;

void ScreenShake_ClearOffsets(void) {
    data_0216e2ac = 0;
    data_0216e2a4 = 0;
}
