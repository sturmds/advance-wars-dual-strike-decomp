#include "types.h"

BOOL SecondScreenUnit_IsStale(void);
void SecondScreen_RedrawPanel(void);
void SecondScreen_DrawTerrainPanel(int a, int b);

extern int data_ov000_02350530;
extern int data_ov000_02350534;
extern int data_ov000_02350538;
extern int data_ov000_0235053c;
extern int data_ov000_02350540;
extern int data_ov000_02350544;
extern int data_ov000_0235054c;

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
