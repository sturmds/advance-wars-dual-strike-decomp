#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Ai_BuildThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_SeekSupply(void);
void func_020af5f8(void);
void AiUnit_WaitInPlace(void);
void AiSeaTransport_Move(void);

/* Queue routine for Black Boats and Landers, set by
 * AiAction_QueueSeaTransports. An empty Lander takes func_020af5f8, the
 * step foot units also run; a loaded Lander or a Black Boat takes
 * AiUnit_SeekSupply. */
void AiSeaTransport_Act(void) {
  Ai_BuildThreatMaps();
  if (GAME->actingUnit->type == UNIT_LANDER &&
      GAME->actingUnit->cargo[0] == 0) {
    func_020af5f8();
  } else {
    AiUnit_SeekSupply();
  }
  AiSeaTransport_Move();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
