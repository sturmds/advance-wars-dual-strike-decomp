#include "types.h"

int func_ov003_023621b0(void);
int Ov003_IsPlayPhase(void);
int func_ov003_02362258(void);
void Ov003_RunSlotInput(void);
void Ov003_UpdateRowUnits(void);
void func_ov003_0235e258(void);
void Ov003_TickUnits(void);
void Ov003_UpdatePickups(void);
void Ov003_UpdateObjects(void);
void Ov003_HealUnitsOnOwnTiles(void);
void Ov003_UpdateTileClaims(void);
void Ov003Rows_TickCountdowns(void);
void Ov003_UpdateSpawns(void);

void func_ov003_023571b4(void)
{
    if (func_ov003_023621b0() == 1) {
        Ov003_RunSlotInput();
    }
    if (Ov003_IsPlayPhase() == 1) {
        Ov003_UpdateRowUnits();
        func_ov003_0235e258();
    }
    if (func_ov003_02362258() != 1) {
        return;
    }
    Ov003_TickUnits();
    Ov003_UpdatePickups();
    Ov003_UpdateObjects();
    Ov003_HealUnitsOnOwnTiles();
    Ov003_UpdateTileClaims();
    Ov003Rows_TickCountdowns();
    Ov003_UpdateSpawns();
}
