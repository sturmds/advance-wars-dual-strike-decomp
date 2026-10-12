#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
BOOL WifiScan_SsidPassesFilter(u32 length, const u8 *ssid);
void WifiRates_DecodeElement(const u8 *element, WlRateSet *rates);

/*
 * Walks the information elements of a received beacon, probe or association
 * frame. Records where the SSID, DS parameter set, rates, TIM, CF parameter
 * set and Nintendo vendor element are, and sets a matched bit for each value
 * that agrees with our own settings (the channel with the join request's).
 */
void WifiRx_ScanElements(WlElements *info)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    const u8 *p = info->data;
    int remaining;
    u32 id;
    u32 length;

    info->unk_12 = link->channel;
    if (info->unk_0c & WIFI_IE_EMPTY_SSID_PASSES) {
        info->result |= WIFI_IE_FITS_SSID;
    }
    for (remaining = info->length; remaining > 0; remaining -= length + 2) {
        id = WL_ReadByte(p++);
        length = WL_ReadByte(p++);
        switch (id) {
        case 0: /* SSID */
            if (length <= WIFI_SSID_LONGEST) {
                info->unk_0c |= WIFI_IE_SEEN_NETWORK_NAME;
                info->unk_18 = p - 2;
                if (length == 0 && (info->unk_0c & WIFI_IE_EMPTY_SSID_PASSES)) {
                    info->result |= WIFI_IE_FITS_SSID;
                    break;
                }
                info->result &= ~WIFI_IE_FITS_SSID;
                /* needed to match: WifiScan_SsidPassesFilter probably takes a u16 length */
                if (WifiScan_SsidPassesFilter((u16)length, p)) {
                    info->result |= WIFI_IE_FITS_SSID;
                }
            } else {
                info->unk_0c &= ~WIFI_IE_SEEN_NETWORK_NAME;
                info->unk_18 = NULL;
            }
            break;
        case 3: /* DS parameter set */
            if (length >= 1) {
                info->unk_0c |= WIFI_IE_SEEN_DS_SET;
                info->unk_12 = WL_ReadByte(p);
                if (info->unk_12 == ((WlJoinRequest *)WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->any.request)->channel) {
                    info->result |= WIFI_IE_FITS_CHANNEL;
                } else {
                    info->result &= ~WIFI_IE_FITS_CHANNEL;
                }
            }
            break;
        case 1: /* supported rates */
            if (length >= 1) {
                info->unk_0c |= WIFI_IE_SEEN_RATE_LIST;
                /* unk_14 and supportedRates sit together as a WlRateSet */
                WifiRates_DecodeElement(p - 2, (WlRateSet *)&info->unk_14);
                if ((info->unk_14 & ~(link->basicRates | link->supportedRates)) == 0
                    && (link->basicRates & (info->unk_14 | info->supportedRates)) == link->basicRates) {
                    info->result |= WIFI_IE_FITS_RATES;
                } else {
                    info->result &= ~WIFI_IE_FITS_RATES;
                }
            }
            break;
        case 5: /* TIM */
            if (length >= 3) {
                info->unk_0c |= WIFI_IE_SEEN_TRAFFIC_MAP;
                info->unk_20 = p - 2;
            }
            break;
        case 4: /* CF parameter set */
            if (length >= 6) {
                info->unk_0c |= WIFI_IE_SEEN_CF_SET;
                info->unk_1c = p - 2;
            }
            break;
        case 221: /* vendor specific */
            if (length >= 8 && WL_ReadByte(p) == 0x00 && WL_ReadByte(p + 1) == 0x09
                && WL_ReadByte(p + 2) == 0xbf && WL_ReadByte(p + 3) == 0) {
                info->unk_0c |= WIFI_IE_SEEN_VENDOR;
                info->nintendo = p - 2;
            }
            break;
        }
        p += length;
    }
    if (info->unk_0c & WIFI_IE_WANT_BSSID_TEST) {
        if ((link->bssid[0] & 1) || (info->unk_04 & 0x8000)) {
            info->result |= WIFI_IE_FITS_BSSID;
        }
    }
    if (info->unk_0c & WIFI_IE_WANT_CAPS_TEST) {
        if ((info->capability & 3) == (link->flags_7c & 3)) {
            info->result |= WIFI_IE_FITS_NET_KIND;
        }
        if ((info->capability & 0x10) == (link->flags_7c & 0x10)) {
            info->result |= WIFI_IE_FITS_PRIVACY;
        }
    }
}
