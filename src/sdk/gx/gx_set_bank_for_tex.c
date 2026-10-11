#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 4 the
 * banks given to texture images. */
extern u16 data_022a9878[];

/* tex is a mask of banks A-D; each bank in it gets the next texture image
 * slot, counting from the lowest bank. With no banks, texturing is switched
 * off in DISP3DCNT. */
void GX_SetBankForTex(int tex)
{
    data_022a9878[0] = ~tex & (data_022a9878[0] | data_022a9878[4]);
    data_022a9878[4] = tex;

    if (tex == 0) {
        /* writes 0 to the two acknowledge bits, which leaves their flags alone */
        *REG_DISP3DCNT &= (u16)~(DISP3DCNT_TEXTURE | DISP3DCNT_UNDERFLOW_ACK | DISP3DCNT_OVERFLOW_ACK);
    } else {
        *REG_DISP3DCNT = (*REG_DISP3DCNT & ~(DISP3DCNT_UNDERFLOW_ACK | DISP3DCNT_OVERFLOW_ACK)) | DISP3DCNT_TEXTURE;
        switch (tex) {
        case GX_VRAM_A | GX_VRAM_C:
            *REG_VRAMCNT_A = VRAMCNT_TEX(0);
            *REG_VRAMCNT_C = VRAMCNT_TEX(1);
            break;
        case GX_VRAM_A | GX_VRAM_D:
            *REG_VRAMCNT_A = VRAMCNT_TEX(0);
            *REG_VRAMCNT_D = VRAMCNT_TEX(1);
            break;
        case GX_VRAM_B | GX_VRAM_D:
            *REG_VRAMCNT_B = VRAMCNT_TEX(0);
            *REG_VRAMCNT_D = VRAMCNT_TEX(1);
            break;
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_D:
            *REG_VRAMCNT_A = VRAMCNT_TEX(0);
            *REG_VRAMCNT_B = VRAMCNT_TEX(1);
            *REG_VRAMCNT_D = VRAMCNT_TEX(2);
            break;
        case GX_VRAM_A | GX_VRAM_C | GX_VRAM_D:
            *REG_VRAMCNT_A = VRAMCNT_TEX(0);
            *REG_VRAMCNT_C = VRAMCNT_TEX(1);
            *REG_VRAMCNT_D = VRAMCNT_TEX(2);
            break;
        case GX_VRAM_D:
            *REG_VRAMCNT_D = VRAMCNT_TEX(0);
            break;
        case GX_VRAM_C | GX_VRAM_D:
            *REG_VRAMCNT_D = VRAMCNT_TEX(1);
        case GX_VRAM_C:
            *REG_VRAMCNT_C = VRAMCNT_TEX(0);
            break;
        case GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
            *REG_VRAMCNT_D = VRAMCNT_TEX(2);
        case GX_VRAM_B | GX_VRAM_C:
            *REG_VRAMCNT_C = VRAMCNT_TEX(1);
        case GX_VRAM_B:
            *REG_VRAMCNT_B = VRAMCNT_TEX(0);
            break;
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
            *REG_VRAMCNT_D = VRAMCNT_TEX(3);
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C:
            *REG_VRAMCNT_C = VRAMCNT_TEX(2);
        case GX_VRAM_A | GX_VRAM_B:
            *REG_VRAMCNT_B = VRAMCNT_TEX(1);
        case GX_VRAM_A:
            *REG_VRAMCNT_A = VRAMCNT_TEX(0);
            break;
        }
    }
    Vram_PowerOnBanks(data_022a9878[0]);
}
