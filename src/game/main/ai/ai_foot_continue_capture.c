#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Ai_BuildThreatMaps(void);
int Unit_CapturePoints(MapUnit* unit);
void AiFoot_PickCaptureTarget(void);
void AiFoot_KeepCapturing(void);
void AiFoot_AttackCapturingUnit(void);

/* Queue routine for infantry and mechs partway through a capture, set by
 * AiAction_QueueCapturingUnits. Starts like AiFoot_MoveAndCapture, then
 * runs AiFoot_KeepCapturing. */
void AiFoot_ContinueCapture(void) {
  Ai_BuildThreatMaps();
  if (Unit_CapturePoints(GAME->actingUnit) >= 20) {
    AiFoot_PickCaptureTarget();
  }
  if (GAME->players[GAME->currentPlayer].aiBehaviour != AI_BEHAVIOUR_DEFENSE) {
    AiFoot_AttackCapturingUnit();
  }
  AiFoot_KeepCapturing();
}
