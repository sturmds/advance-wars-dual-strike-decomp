#include "types.h"

/* One's complement sum of size / 2 halfwords, as the beacon checksum uses. */
u16 MbBeacon_Checksum(const u16 *data, int size)
{
    u32 sum = 0;
    int count;

    for (count = size >> 1; count > 0; count--) {
        sum += *data++;
    }
    sum = (sum & 0xffff) + (sum >> 16);
    sum += sum >> 16;
    return sum ^ 0xffff;
}
