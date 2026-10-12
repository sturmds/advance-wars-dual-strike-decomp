#include "types.h"
#include "game/unit_info_panel.h"

/* Clears the panel's unit. */
void UnitInfoPanel_ClearUnit(void) { data_ov000_0234ec8c->unit = NULL; }
