#include "types.h"
#include "sdk/gx.h"

#define DISP3DCNT_ACKS (DISP3DCNT_UNDERFLOW_ACK | DISP3DCNT_OVERFLOW_ACK)

/* Startup setup of the 3D engine: empties the geometry FIFO, waits until the
 * engine is idle, clears DISP3DCNT and the status flags, turns on
 * anti-aliasing, resets the matrix stacks, clear color, depth and fog, and
 * loads the default tables and polygon attributes. */
void G3X_Init(void)
{
    G3X_ClearFifo();
    *REG_END_VTXS = 0;
    while (*REG_GXSTAT & GXSTAT_GEOMETRY_BUSY) {
    }
    *REG_DISP3DCNT = 0;
    *REG_GXSTAT = 0;
    *IO_BG0_SCROLL = 0;

    *REG_DISP3DCNT |= DISP3DCNT_UNDERFLOW_ACK;
    *REG_DISP3DCNT |= DISP3DCNT_OVERFLOW_ACK;
    *REG_DISP3DCNT &= ~(DISP3DCNT_HIGHLIGHT_SHADING | DISP3DCNT_ACKS);
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~DISP3DCNT_ACKS) | DISP3DCNT_ANTI_ALIAS;
    *REG_DISP3DCNT &= (u16)~(DISP3DCNT_ALPHA_TEST | DISP3DCNT_ACKS);   /* needed to match: 16-bit mask literal */

    *REG_GXSTAT |= GXSTAT_MTX_STACK_ERROR_ACK;
    *REG_GXSTAT = (*REG_GXSTAT & ~GXSTAT_FIFO_IRQ_MASK) | GXSTAT_FIFO_IRQ_EMPTY;
    G3X_InitMtxStack();

    *REG_CLEAR_COLOR = 0;
    *REG_CLEAR_DEPTH = 0x7fff;
    *IO_3D_REAR_PLANE_SCROLL = 0;
    *REG_FOG_COLOR = 0;
    *REG_FOG_OFFSET = 0;
    *REG_BG0CNT &= ~3;   /* BG0, which shows the 3D layer, gets priority 0 */
    G3X_InitTable();

    *REG_POLYGON_ATTR = 0x001f0080;
    *REG_TEXIMAGE_PARAM = 0;
    *REG_PLTT_BASE = 0;
}
