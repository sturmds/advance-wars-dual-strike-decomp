#include "types.h"

int func_ov003_023621b0(void);
int func_ov003_02362174(void);
int func_ov003_02362258(void);
void Ov003_RunSlotInput(void);
void func_ov003_0235e9f0(void);
void func_ov003_0235e258(void);
void Ov003_TickUnits(void);
void func_ov003_0235de88(void);
void Ov003_UpdateObjects(void);
void Ov003_HealUnitsOnOwnTiles(void);
void Ov003_UpdateTileClaims(void);
void func_ov003_0235d7a4(void);
void func_ov003_0235bb64(void);

void func_ov003_023571b4(void)
{
    if (func_ov003_023621b0() == 1) {
        Ov003_RunSlotInput();
    }
    if (func_ov003_02362174() == 1) {
        func_ov003_0235e9f0();
        func_ov003_0235e258();
    }
    if (func_ov003_02362258() != 1) {
        return;
    }
    Ov003_TickUnits();
    func_ov003_0235de88();
    Ov003_UpdateObjects();
    Ov003_HealUnitsOnOwnTiles();
    Ov003_UpdateTileClaims();
    func_ov003_0235d7a4();
    func_ov003_0235bb64();
}
