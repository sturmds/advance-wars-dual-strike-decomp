#include "types.h"
#include "game/ov003_unit.h"

int func_ov003_0235da18(int rowNumber);

int func_ov003_0235d468(int rowNumber)
{
    if (func_ov003_0235da18(rowNumber) == 1) {
        /* needed to match: a one-case switch, not an if */
        switch (data_ov003_02364928->rows[rowNumber - 1].unk_3b_0) {
        case 1:
            return 1;
        }
    }
    return 0;
}
