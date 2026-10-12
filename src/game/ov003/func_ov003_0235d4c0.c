#include "types.h"
#include "game/ov003_unit.h"

int Ov003_IsRowPowerActive(int rowNumber);

int func_ov003_0235d4c0(int rowNumber) {
  if (Ov003_IsRowPowerActive(rowNumber) == 1) {
    /* needed to match: a one-case switch, not an if */
    switch (data_ov003_02364928->rows[rowNumber - 1].co) {
      case 3:
        return 1;
    }
  }
  return 0;
}
