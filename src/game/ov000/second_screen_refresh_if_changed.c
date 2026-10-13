#include "types.h"
#include "game/second_screen.h"

BOOL SecondScreenUnit_IsStale(void);
void SecondScreen_RedrawPanel(void);
void SecondScreen_DrawTerrainPanel(int a, int b);

int data_ov000_02350530;
int data_ov000_02350538;
int data_ov000_02350534;
int data_ov000_0235054c;
int data_ov000_0235053c;
int data_ov000_02350540;
int data_ov000_02350544;
int data_ov000_02350548;
u8 data_ov000_02350550[0x10]; /* 0x10 bytes no code names, before the end */

/* Second screen: when SecondScreenUnit_IsStale reports that the requested values differ from the
 * drawn ones, copies them over and redraws, then calls SecondScreen_DrawTerrainPanel either way. */
void SecondScreen_RefreshIfChanged(void) {
  if (SecondScreenUnit_IsStale()) {
    data_ov000_02350530 = data_ov000_02350538;
    data_ov000_0235054c = data_ov000_02350534;
    data_ov000_0235053c = data_ov000_02350544;
    SecondScreen_RedrawPanel();
  }
  SecondScreen_DrawTerrainPanel(data_ov000_02350540, data_ov000_02350530);
}
