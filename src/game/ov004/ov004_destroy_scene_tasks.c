#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Task_Destroy(struct Task* task);

void Ov004_DestroySceneTasks(Unk_ov004_022bf158* state) {
  if (state->unk_1bf8 != NULL) {
    Task_Destroy(state->unk_1bf8);
  }
  if (state->unk_1bfc != NULL) {
    Task_Destroy(state->unk_1bfc);
  }
  if (state->unk_1c00 != NULL) {
    Task_Destroy(state->unk_1c00);
  }
  if (state->unk_1c04 != NULL) {
    Task_Destroy(state->unk_1c04);
  }
  if (state->unk_1bf4 != NULL) {
    Task_Destroy(state->unk_1bf4);
  }
  if (state->unk_1c08 != NULL) {
    Task_Destroy(state->unk_1c08);
  }
}
