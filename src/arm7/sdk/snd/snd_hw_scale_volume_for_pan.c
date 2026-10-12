#include "sdk/snd_hw.h"

int SndHw_ScaleVolumeForPan(int volume, int pan)
{
    if (pan < 24) {
        return (volume * (gSndHwPanEdgeAttenuation * (pan + 40) + (0x7FFF - gSndHwPanEdgeAttenuation) * 64)) >> 21;
    }
    if (pan > 104) {
        return (volume * (-gSndHwPanEdgeAttenuation * (pan - 40) + (gSndHwPanEdgeAttenuation + 0x7FFF) * 64)) >> 21;
    }
    return volume;
}
