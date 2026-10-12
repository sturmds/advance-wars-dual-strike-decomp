/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/fs.h"
#include "sdk/isd.h"

extern char gIsdSizePositiveCheck[];
extern char gIsdSizeLimitCheck[];

/*
 * Adds the range [address, address + size), loaded from fileOffset, to target's
 * overlay table, replacing whatever the table held for that range. Returns FALSE
 * when target has no table.
 */
BOOL Isd_OverlayTableAddRange(MIProcessor target, u32 address, u32 fileOffset,
                              u32 size) {
  IsdRegionState saved;
  BOOL found;
  u64 end;
  u32 i;
  IsdOverlayInfo* table =
      gIsdOverlayTables[target]; /* needed to match: declared last */

  if (table == NULL) {
    return FALSE;
  }
  if (size == 0) {
    return TRUE;
  }
  Isd_OpenDebugRegion(&saved);
  while (Isd_SwapWord(ISD_OVERLAY_LOCKED, &table->header.lock) != 0) {
  }
  table->header.flags |= ISD_OVERLAY_FLAG_UPDATING;
  /* A range that runs past the top of the address space stops there. */
  if ((u64)address + size > 0x100000000ULL) {
    size = 0x100000000ULL - address;
  }
  end = (u64)address + size;
  found = FALSE;
  for (i = 0; i < table->header.infoCount && !found; i++) {
    u32 vma = table->info[i].vma;

    if (vma > address) {
      /* The range starts before entry i: put a new entry there. */
      found = TRUE;
      Isd_OverlayTableInsert(table, i);
      table->info[i].vma = address;
      table->info[i].fileOffset = fileOffset;
      table->info[i].size = size;
      table->info[i].unk_0C = 0;
      /* Remove the entries after it that the range covers, and trim the one it ends in. */
      while (i + 1 < table->header.infoCount) {
        s64 wrapSize;

        if ((u64)table->info[i + 1].vma + table->info[i + 1].size <= end) {
          Isd_OverlayTableRemove(table, i + 1);
          continue;
        }
        wrapSize = end - table->info[i + 1].vma;
        if (wrapSize <= 0) {
          break;
        }
        if (!(table->info[i + 1].size > (u32)wrapSize)) {
          Isd_ReportFailure(gIsdOverlayFileName, 345, gIsdWrapSizeCheck, TRUE);
        }
        table->info[i + 1].vma += (u32)wrapSize;
        table->info[i + 1].fileOffset += (u32)wrapSize;
        table->info[i + 1].size -= (u32)wrapSize;
        if (!(table->info[i + 1].size > 0)) {
          Isd_ReportFailure(gIsdOverlayFileName, 351, gIsdSizePositiveCheck,
                            TRUE);
        }
        if (!(table->info[i + 1].size < 0xf0000000)) {
          Isd_ReportFailure(gIsdOverlayFileName, 352, gIsdSizeLimitCheck, TRUE);
        }
        break;
      }
    } else if (vma <= address && address < (u64)vma + table->info[i].size) {
      /* The range starts inside entry i. */
      if (vma > address) {
        Isd_ReportFailure(gIsdOverlayFileName, 358, gIsdEntryStartCheck, TRUE);
      }
      if (!((u64)table->info[i].vma + table->info[i].size > address)) {
        Isd_ReportFailure(gIsdOverlayFileName, 359, gIsdEntryEndCheck, TRUE);
      }
      if (address != table->info[i].vma) {
        /* Split the entry at address: the first copy keeps the part below it. */
        Isd_OverlayTableInsert(table, i);
        table->info[i] = table->info[i + 1];
        table->info[i].size = address - table->info[i].vma;
        table->info[i + 1].vma += table->info[i].size;
        table->info[i + 1].fileOffset += table->info[i].size;
        table->info[i + 1].size -= table->info[i].size;
        i++;
      }
      if (address != table->info[i].vma) {
        Isd_ReportFailure(gIsdOverlayFileName, 372, gIsdEntryAtAddressCheck,
                          TRUE);
      }
      if (table->info[i].size <= size) {
        /* Entry i becomes the new range; later entries it covers go. */
        found = TRUE;
        table->info[i].fileOffset = fileOffset;
        table->info[i].size = size;
        while (i + 1 < table->header.infoCount) {
          s64 wrapSize;

          if ((u64)table->info[i + 1].vma + table->info[i + 1].size <= end) {
            Isd_OverlayTableRemove(table, i + 1);
            continue;
          }
          wrapSize = end - table->info[i + 1].vma;
          if (wrapSize <= 0) {
            break;
          }
          if (!(table->info[i + 1].size > (u32)wrapSize)) {
            Isd_ReportFailure(gIsdOverlayFileName, 393, gIsdWrapSizeCheck,
                              TRUE);
          }
          table->info[i + 1].vma += (u32)wrapSize;
          table->info[i + 1].fileOffset += (u32)wrapSize;
          table->info[i + 1].size -= (u32)wrapSize;
        }
      } else {
        /* Entry i is longer than the range: keep its tail, put the range before it. */
        found = TRUE;
        table->info[i].vma += size;
        table->info[i].fileOffset += size;
        table->info[i].size -= size;
        Isd_OverlayTableInsert(table, i);
        table->info[i].vma = address;
        table->info[i].fileOffset = fileOffset;
        table->info[i].size = size;
        table->info[i].unk_0C = 0;
      }
    }
  }
  if (!found) {
    Isd_OverlayTableInsert(table, i);
    table->info[i].vma = address;
    table->info[i].fileOffset = fileOffset;
    table->info[i].size = size;
    table->info[i].unk_0C = 0;
  }
  table->header.flags &= ~ISD_OVERLAY_FLAG_UPDATING;
  table->header.lock = 0;
  Isd_CloseDebugRegion(&saved);
  return TRUE;
}
