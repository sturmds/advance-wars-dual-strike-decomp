#include "types.h"
#include "game/camera_pan.h"
#include "game/camera_preset.h"
#include "game/campaign_recap.h"

extern Camera data_022a7d5c;
extern int gCameraPresetIndex;

void Camera_ResetToPreset0(Camera* camera);
s32 HwDiv32(s32 numerator, s32 denominator);

/* Puts the camera at the current preset's position moved by x - 50, y and z
 * hundredths of a map tile (0x200 world units). recap is not read. */
void CampaignRecap_SetCamera(CampaignRecap* recap, int x, int y, int z) {
  int cameraX;
  int cameraY;
  int cameraZ;

  Camera_ResetToPreset0(&data_022a7d5c);
  cameraX = gCameraPresets[gCameraPresetIndex].x + HwDiv32((x - 50) << 9, 100);
  cameraY = gCameraPresets[gCameraPresetIndex].y + HwDiv32(y << 9, 100);
  cameraZ = gCameraPresets[gCameraPresetIndex].z + HwDiv32(z << 9, 100);
  data_022a7d5c.x = cameraX;
  data_022a7d5c.y = cameraY;
  data_022a7d5c.z = cameraZ;
}
