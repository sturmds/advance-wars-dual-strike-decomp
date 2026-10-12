#include "types.h"

int Ov003_IsPhase0Or1(void);
int Ov003_IsPlayPhase(void);
int Ov003_IsPhase0OrAbove6(void);
void Ov003_RunSlotInput(void);
void Ov003_UpdateRowUnits(void);
void Ov003_StepMarkers(void);
void Ov003_TickUnits(void);
void Ov003_UpdatePickups(void);
void Ov003_UpdateObjects(void);
void Ov003_HealUnitsOnOwnTiles(void);
void Ov003_UpdateTileClaims(void);
void Ov003Rows_TickPowers(void);
void Ov003_UpdateSpawns(void);

void func_ov003_023571b4(void) {
  if (Ov003_IsPhase0Or1() == 1) {
    Ov003_RunSlotInput();
  }
  if (Ov003_IsPlayPhase() == 1) {
    Ov003_UpdateRowUnits();
    Ov003_StepMarkers();
  }
  if (Ov003_IsPhase0OrAbove6() != 1) {
    return;
  }
  Ov003_TickUnits();
  Ov003_UpdatePickups();
  Ov003_UpdateObjects();
  Ov003_HealUnitsOnOwnTiles();
  Ov003_UpdateTileClaims();
  Ov003Rows_TickPowers();
  Ov003_UpdateSpawns();
}
