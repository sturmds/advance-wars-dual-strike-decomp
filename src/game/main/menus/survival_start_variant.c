#include "types.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

void Survival_StartTimeMode(void);
void Survival_StartDaysMode(void);
void Survival_StartFundsMode(void);

/* Starts a Survival run by the variant in data_022a7b80.mode: 0 and 3,
 * 1 and 4, and 2 and 5 pair up (as in Survival_FinishMap), each pair with its
 * own setup routine. */
void Survival_StartVariant(void) {
  switch (data_022a7b80.mode) {
    case 1:
    case 4:
      Survival_StartFundsMode();
      break;
    case 2:
    case 5:
      Survival_StartDaysMode();
      break;
    case 0:
    case 3:
      Survival_StartTimeMode();
      break;
  }
}
