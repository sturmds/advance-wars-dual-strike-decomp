# Tool issues

Problems found in third-party tools, with what we do about them. Kept local; nothing here has been reported upstream.

## dsd: `init` aborts on AWRE

Tracked as `docs/issues/0007-dsd-init-abort.md`.

- **Versions:** dsd 0.12.0 and 0.12.1 (`tools.lock`). The unreleased commits after v0.12.1 at `c408063` change the disassembler, not `init`'s analysis, so a source build is not expected to help (not tested: building it needs cargo).
- **Command:** `dsd init -r extract/config.yaml -o <out> -b build`
- **Error:**
  ```
  [ERROR] Local function call from 0x021347a6 in ARM9 main to 0x021341aa leads to no function
  ```
- **Cause:** after the last function in main's `.text` (ends 0x02129760), `init` keeps looking for function starts for up to 0x2000 bytes (`max_function_start_search_distance` in `lib/src/config/module.rs`). The bytes after `.text` are `.rodata`, starting with the MSL exception table and then graphics data. They decode as plausible Thumb code up to 0x02134968, and a `bl` in that fake code targets an address with no function, which is a hard error.
- **Workaround:** `tools/dsd_init.py` runs `init` on a temporary copy of the extract with 0x2100 bytes zeroed at 0x02129760, then points the config at the real extract and writes main's real code hash (`tools/fxhash.py`).
- **Cost of the workaround:** `init` records no pointers in 0x02129760–0x0212b860. The 29 real ones (exception table) were added to `config/AWRE/arm9/relocs.txt` by hand. Rerunning `dsd_init.py --force` drops them; restore them from git.
- **Possible upstream fix:** stop the search at the end of `.text` when the module's section boundaries are known, or turn "leads to no function" into a warning for addresses outside `.text`.

## dsd: `rom build` without an ARM7 BIOS

- **Behaviour:** without `--arm7-bios`, `dsd rom build` writes 0 for the secure-area CRC (header 0x6C), so the header CRC (0x15E) differs too. 4 bytes in total.
- **Workaround:** `tools/fix_header.py` checks that our secure area (0x4000–0x7FFF) equals the original's, copies the original secure-area CRC, and recomputes the header CRC. No BIOS dump needed.
- **Not a bug:** the CRC covers the encrypted secure area, and the encryption key comes from the BIOS.

## dsd: smaller warnings

- `init` warns 28 times about calls into the middle of functions, all in runtime and floating-point assembly in 0x0211e980–0x02129760. It adds labels for them; the build is unaffected.
- `dsd fix find-exceptix` reports "__FindExceptionTable not found". Not needed for M0.
- `dsd objdiff` names the top-level `target_dir` and `base_dir` the other way round from objdiff's convention. The per-unit `target_path` (original) and `base_path` (ours) are correct, and those are what objdiff uses.

## mwldarm: link time grows with objects squared times path length

- **Version:** mwldarm 2.0-72 (metroskrew 0.1.3).
- **Seen:** 2026-10-10, the link took 12 s of a 12.6 s landing build; compile, delink and ROM build together under 1 s, and the `make check` steps under 0.1 s.
- **Cause:** the linker compares each object's full path (the shim's `GetFullPathNameA` puts `getcwd()` in front of relative paths) with every object loaded before it. Measured with synthetic objects: 1,000 / 2,000 / 4,000 objects with one function each link in 0.5 / 2.1 / 7.6 s, while 250 objects with 32 functions each link in 0.07 s, so the cost follows the object count, not the symbol count. Adding 90 characters to the directory name raised 2,000 objects from 1.9 s to 3.3 s. A worktree path plus `build/delinks/_dsd_gap@main_` gives a common prefix of about 105 characters. The map file (`-map`) and the 3,000 `.exceptix` warnings per link cost under 0.5 s.
- **Workaround:** `tools/link.py` makes `/tmp/awlXXXXXXXX`, symlinks each top-level directory of `build/` into it under a one-character name, and runs the linker there with short paths; it removes the directory afterwards. The ELF and every `.bin` are byte-identical. In a worktree with 5,073 objects on a loaded machine the link went from 11.1-12 s to 4.1-4.2 s; `make release` after a change from about 12.6 s to 4.7 s.
- **Still open:** the cost keeps growing with the square of the object count as functions get their own files. Fewer, larger objects (several functions per C file) would cut it further; that is a source layout choice for sturmds.

## dsd: missed an ITCM load relocation in func_0201c8ec

- **Seen:** 2026-10-11, in `func_0201c8ec` (`EventScript_ShowPicture`). dsd did not find the load at `0x0201c9f0` that points into ITCM at `0x01ff8f64`, so the build had no relocation for it.
- **Workaround:** batch 21 group `m01` added `from:0x0201c9f0 kind:load to:0x01ff8f64 module:itcm` to `relocs.txt` and `data_01ff8f64` to the ITCM `symbols.txt` by hand (landed in `7d9ec6c78`). Look for the same miss in other functions that read ITCM data.
