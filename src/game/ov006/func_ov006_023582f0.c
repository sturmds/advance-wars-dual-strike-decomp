#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"

int DesignMap_CopyName(u8 index, u8* dest);
void Ov006_ClearLayer0Box(void);
void Ov006_ClearWindowsAndBlend(void);
void DesignMap_DrawSlotPreview(int a, int b, int c, int d, u8 e);
void Ov006_DarkenPreviewBox(void);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern Unk_027c0280* data_027c0280;

/* Clears the layer 0 box. If design map slot id holds a map, draws it through
 * DesignMap_DrawSlotPreview and resets the layer 0 scroll; otherwise clears windows and blending. */
void func_ov006_023582f0(int id) {
  Ov006_ClearLayer0Box();
  if (DesignMap_CopyName(id, &data_ov006_0236d640->unk_eb) == 1) {
    DesignMap_DrawSlotPreview(16, 5, 68, 7, id);
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    Ov006_DarkenPreviewBox();
  } else {
    Ov006_ClearWindowsAndBlend();
  }
}
