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

## dsd: relocations to overlays that share an address

- **Behaviour:** when a pointer targets an address that several overlays cover, `dsd init` writes `module:overlays(A,B,...)` and adds a symbol marked `ambiguous` in each candidate. `dsd delink` then names the first candidate's symbol in the object. The matching build links the same bytes either way; a shifted build would link to the first candidate even when another one is meant.
- **What we do:** `tools/resolve_overlay_relocs.py` narrows them by rule (issue 0019); 15 are left (issue 0055).

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

## dsd: no ARM7 programs, and no branches above 0x03000000

Tracked as `docs/issues/0013-arm7-from-objects.md` (closed by the workaround).

- **Version:** dsd 0.12.1 (`tools.lock`), and the source at `c408063`.
- **What is missing:** `init`, `delink`, `lcf` and `objdiff` read only the ARM9 side of a ROM: a `config.yaml` has a main module, autoloads and ARM9 overlays, nothing for ARM7. Pointing them at ARM7 as if it were ARM9 fails in two ways:
  - ARM9 analysis expects ARM9's crt0: a 0x800-byte secure area, a static-initializer runner whose first literal points at `.ctor`, and build info ending in the `0xdec00621` "nitrocode" words. ARM7's crt0 has none of these (`expected nitrocode ... at the end of build info`, then a panic in `lib/src/analysis/ctor.rs`).
  - The function parser treats any branch to an address outside 0x01ff8000-0x03000000 as illegal (`lib/src/analysis/functions.rs`, "branch outside of program"). ARM7 WRAM is at 0x037f8000. `init` stops at the first such branch, and `delink` fails on any function in `symbols.txt` that has one: `function func_037f802c could not be analyzed: IllegalIns { ..., reason: "branch outside of program" }`. Code in main RAM calls WRAM 784 times, so nearly every ARM7 function is affected.
  - An autoload at 0x027c0000 is taken for DTCM, and DTCM gets no function search.
- **Workaround:** `tools/arm7.py` gives dsd a stand-in ROM: a made-up ARM9 main module that passes the ARM9 checks, empty ITCM and DTCM, and the three ARM7 parts as unknown autoloads, with WRAM moved to 0x02a00000 and main RAM to 0x02c00000. At each relocation in `config/AWRE/arm7/relocs.txt` it rewrites the original word for the moved addresses, and writes a dsd config with the same addresses moved. `dsd delink` and `dsd lcf` run on that. The link script gets the real addresses back, so mwldarm places every object where it belongs and resolves every relocation to its real target; `build/build/arm7.bin` equals the retail ARM7 byte for byte. `tools/arm7_init.py` generated the config the same way, once.
- **What dsd's analysis still got wrong on ARM7,** fixed by `arm7_init.py`: it stopped searching at the first data in WRAM and missed the functions after it, split the Thumb BIOS call stubs at multiples of 4, and took a table of function pointers at 0x027d6570 for a Thumb function.
- **Upstream fix:** ARM7 modules in the config, and the branch range taken from the module list instead of a constant.

## dsd and mwldarm: objects that start at 2 mod 4

- **Version:** dsd 0.12.1 (`tools.lock`), mwldarm 2.0-72.
- **Seen:** 2026-10-11, with the BIOS call stubs in main (0x02000000-0x020007ff). Ten of these Thumb stubs start at an address that is 2 mod 4, or follow a 6-byte stub.
- **Behaviour:** an object that starts at 2 mod 4 is placed at the next multiple of 4, which moves every later byte and breaks the secure-area check in `tools/fix_header.py`. Three things cause it or block a fix:
  - `dsd lcf` writes `ALIGNALL(4);` at the top of the `.arm9` block (`assets/arm9.lcf.template` in ds-decomp).
  - Every object's `.text` has `sh_addralign` 4: mwccarm writes 4 even for a Thumb `asm` function, and `dsd delink` writes 4 for every code section (`cli/src/cmd/delink.rs`), gaps included.
  - A delinks.txt file entry cannot set its own alignment: `align:2` is refused with "attribute 'align' should be omitted as it is inherited from this file's header" (`lib/src/config/section.rs`).
- **What mwldarm does:** it aligns the start of each object to the larger of `ALIGNALL` and the object's `sh_addralign`, and does not pad the end. Tested by hand: with only one of the two set to 2, the stub still moved to the next multiple of 4.
- **Workaround:** `tools/lcf_align2.py`. After `dsd lcf`, it puts `ALIGNALL(2);` ... `ALIGNALL(4);` around each main `.text` object that starts at 2 mod 4: a file whose delinks range starts there, and the object after a file whose range ends there (usually a gap). Before the link it copies those objects to `build/align2/` with `.text` alignment 2, and the linker reads the list in `build/objects.link.txt`. Every other object keeps ALIGNALL(4) and its own alignment. Only ARM9 main is handled; ITCM, DTCM and the overlays have no such object yet.
- **Other projects:** khdays-decomp turns every `ALIGNALL(4)` into `ALIGNALL(2)` and sets `.text` alignment 2 in every object (`tools/_run_mwld.py`, `tools/patch_align.py`). That changes the alignment of every object; ours changes only the ones that need it.
