#include "types.h"
#include "game/task.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/camera_pan.h"

void func_020d796c(void);
void Camera_ResetToPreset0(void* camera);
void Task_Destroy(Task* task);
void Mem_Free(void* block);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern Camera data_022a7d5c;

/* Undoes DesignRoom_Init: resets the camera, stops the second overlay task
 * and frees the overlay state if it lies in main RAM (0x02000000-0x02800000). */
void DesignRoom_Exit(void) {
  func_020d796c();
  Camera_ResetToPreset0(&data_022a7d5c);
  if (data_ov006_0236d640->unk_60 != NULL) {
    Task_Destroy(data_ov006_0236d640->unk_60);
  }
  if ((u32)data_ov006_0236d640 < 0x02000000) {
    return;
  }
  if ((u32)data_ov006_0236d640 > 0x02800000) {
    return;
  }
  Mem_Free(data_ov006_0236d640);
  data_ov006_0236d640 = NULL;
}
