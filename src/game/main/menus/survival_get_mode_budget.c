#include "types.h"

extern int gSurvivalModeBudgets[];

/* Full budget of a Survival mode (0-5): seconds, funds or days. */
int Survival_GetModeBudget(int mode) { return gSurvivalModeBudgets[mode]; }
