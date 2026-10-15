# Every target goes through here (D2). After cloning and linking your ROM at
# orig/AWRE.nds: `make setup` (tools, hooks, and the map files and other data
# unpacked from your ROM into extracted/), then `make` and `make check`.
# `make release` runs all of it.

ROM      := orig/AWRE.nds
ROM_SHA1 := config/AWRE/rom.sha1
MODULES_SHA1 := config/AWRE/modules.sha1
DSD_CONFIG := config/AWRE/arm9/config.yaml
OUT      := build/AWRE.nds
PYTHON   ?= python3
NINJA    ?= ninja
DSD      := .cache/tools/dsd/dsd
# One ninja run at a time per checkout. Two runs in one build/ delete each
# other's delinked objects (dsd delink starts with rm -rf build/delinks), and
# the link then misses a _dsd_gap@...o file (issue 0025). tools/fn.py takes
# the same lock.
LOCKED_NINJA := mkdir -p build && flock build/.ninja.lock $(NINJA)


.PHONY: all configure fnindex dsindex progress setup tools assets release release-nocheck decomp-dev-report pnc-check check check-rom check-modules check-out hooks extract sound music test test-release clean shift emu-check format format-check

all: check-rom configure
	@$(LOCKED_NINJA)

# Cheap, and rewrites build.ninja only when it changes.
configure:
	@$(PYTHON) tools/configure.py

# Tools and hooks, then the game data the build needs from your ROM (docs/maps.md).
setup: tools assets

# Downloads tools.lock entries into .cache/ once; offline after that.
tools: hooks
	@$(PYTHON) tools/setup.py

# Map files, the data in code sections, the sound archive and the text are
# game data: unpacked from the ROM into extracted/, never committed
# (docs/maps.md; docs/workflow.md, "Data from the ROM"; docs/sound.md;
# docs/text.md). The build also rewrites the data files when they are stale.
assets: tools extract
	@$(MAKE) --no-print-directory dsindex
	@$(PYTHON) tools/maps.py unpack
	@$(PYTHON) tools/data_inc.py write
	@$(PYTHON) tools/sound.py unpack
	@$(PYTHON) tools/sound.py export --auto
	@$(PYTHON) tools/text.py unpack

# The sound archive's files and sound.yaml in extracted/AWRE/assets/sound/.
# The ROM build still copies sound_data.sdat from extract/ (issue 0050).
sound: extract
	@$(PYTHON) tools/sound.py unpack

# Every song, sound effect and stream as MP3 in extracted/AWRE/music/
# (docs/sound.md, "Export"). Needs ffmpeg. `make assets` runs it too, except
# in a linked worktree or without ffmpeg; it skips files already up to date.
music: tools extract
	@$(PYTHON) tools/sound.py export

hooks:
	@if [ "$$(git config --get core.hooksPath)" != hooks ]; then \
		git config core.hooksPath hooks && echo "setup: git hooks installed (core.hooksPath=hooks)"; \
	fi

extract: check-rom configure
	@$(LOCKED_NINJA) extract/config.yaml

check-rom:
	@if [ ! -e $(ROM) ]; then \
		echo "error: $(ROM) not found. Link your dump there, for example:" >&2; \
		echo "  mkdir -p orig && ln -s /path/to/your/awds.nds $(ROM)" >&2; \
		exit 1; \
	fi
	@sha1sum --quiet --strict -c $(ROM_SHA1) || { echo "error: $(ROM) is not the US/AUS AWRE retail ROM" >&2; exit 1; }

# Per-module checks first, so a mismatch names the module. dsd checks the ARM9
# modules; modules.sha1 also covers build/build/arm7.bin.
check-modules:
	@$(DSD) check modules -c $(DSD_CONFIG) 2>&1 | grep -v ': OK$$' | grep -v '^\[INFO \] Loading' || true
	@sha1sum --quiet --strict -c $(MODULES_SHA1)

check-out:
	@echo "$$(cut -d' ' -f1 $(ROM_SHA1))  $(OUT)" | sha1sum --quiet --strict -c - \
		|| { echo "error: $(OUT) does not match the retail ROM" >&2; exit 1; }
	@echo "check: $(OUT) matches retail AWRE (SHA-1 $$(cut -d' ' -f1 $(ROM_SHA1)))"

check: check-rom check-modules check-out

# build/fnindex.json, for fn.py check and the brute-force tools. fn.py check
# also rebuilds it when symbols.txt, relocs.txt or extract/ are newer.
fnindex: all
	@$(PYTHON) tools/fnindex.py

# build/dsindex.json, struct layouts for tools/ds.py from probe compiles of
# each header. ds.py also refreshes it when a header or C file is newer.
dsindex:
	@$(PYTHON) tools/dsindex.py

# The decomp.dev measures as PROGRESS.md shows them, printed.
progress: decomp-dev-report
	@$(PYTHON) tools/progress.py

# clang-format on src/ and include/, style in .clang-format. format rewrites
# files; format-check only reports and is part of release.
format: tools
	@$(PYTHON) tools/format.py

format-check: tools
	@$(PYTHON) tools/format.py --check

# Tenet 8: no tracked file matches a private pattern (.git/info/pnc-patterns).
pnc-check:
	@$(PYTHON) tools/pnc_check.py

# The first and last steps record a tree that passed, for hooks/pre-commit
# and land_self.py (tools/release_cache.py). The test step skips the tests in
# CACHED_TESTS when they already passed on the same tools (test-release).
release: setup
	@$(PYTHON) tools/release_cache.py begin
	@$(MAKE) --no-print-directory pnc-check
	@$(MAKE) --no-print-directory format-check
	@$(MAKE) --no-print-directory all
	@$(MAKE) --no-print-directory check
	@$(MAKE) --no-print-directory test-release
	@$(MAKE) --no-print-directory decomp-dev-report
	@$(PYTHON) tools/release_cache.py record
	@echo "release: OK"

# release without check: for a build that is not meant to match retail yet.
# The input ROM is still checked against rom.sha1 (check-rom), since every
# step reads it.
release-nocheck: setup
	@$(MAKE) --no-print-directory pnc-check
	@$(MAKE) --no-print-directory format-check
	@$(MAKE) --no-print-directory all
	@$(MAKE) --no-print-directory test-release
	@$(MAKE) --no-print-directory decomp-dev-report
	@echo "release-nocheck: OK (build/AWRE.nds not compared with retail)"

# build/artifacts/AWRE_report/report.json: the objdiff report over both CPUs,
# laid out as decomp.dev's artifact. hooks/pre-commit commits a copy as
# PROGRESS.json, which .github/workflows/decomp-dev.yml uploads.
decomp-dev-report: all
	@$(PYTHON) tools/decomp_dev_report.py

# Shifted ROMs (issues 0012, 0086): 0x20 bytes inserted in main (build/shift/),
# then in each overlay, itcm and dtcm, one at a time, then 0x40 in main and
# ov000 together (the case of the arena-lo crash). Each checks symbol moves,
# the arena start and words that kept an old address. Not part of release.
shift: all
	@flock build/.ninja.lock $(PYTHON) tools/shift_build.py
	@for m in ov000 ov001 ov002 ov003 ov004 ov005 ov006; do \
		flock build/.ninja.lock $(PYTHON) tools/shift_build.py --module $$m || exit 1; \
	done
	@flock build/.ninja.lock $(PYTHON) tools/shift_build.py --module itcm
	@flock build/.ninja.lock $(PYTHON) tools/shift_build.py --module dtcm
	@flock build/.ninja.lock $(PYTHON) tools/shift_build.py --module main --module ov000 --pad 0x40

# Boots retail, our build and every shifted build from make shift in melonDS
# (env MELONDS, a local build) with no window and no sound, opens the main
# menu, and fails on an ARM9 exception, a hang, a heap that starts inside
# overlay memory, or a screen retail reaches and the ROM does not (issue 0006,
# tools/melonds_check.py). Not part of release.
SHIFT_ROMS := build/shift/AWRE.nds build/shift-ov000/AWRE.nds build/shift-itcm/AWRE.nds \
	build/shift-dtcm/AWRE.nds build/shift-main-ov000/AWRE.nds
emu-check: shift
	@$(PYTHON) tools/melonds_check.py $(ROM) $(OUT) $(SHIFT_ROMS)

# Part of release, so a landing runs it too. A test that needs something
# release does not (gcc, decomp-permuter, a ROM) prints "skipped" and passes.
# Tests of the landing tools on throwaway repos, of propagate.py's C text
# handling, of ds.py, of pragmas.py, of absolute symbols, of the rows fn.py diff
# marks = (test_diff_rows.py), of 2-byte alignment
# (tools/lcf_align2.py), of ARM7's address moves (tools/arm7.py) and of how
# partials are ranked (test_ranking.py), and the plain C beside asm functions
# compiled with the host gcc (test_portable_c.py), and the PROGRESS.md rows
# progress.py writes from a report (test_progress.py), and the order in which
# mwccarm emits data objects (test_data_order.py), and the .bss planner
# (test_bss.py), the release cache (test_release_cache.py) and the .data
# planner (test_data.py), and the sound archive's unpack and pack
# (test_sound.py), and the levels of scan_pointers.py (test_scan_pointers.py),
# and the C text shift_gaps.py counts as a gap (test_shift_gaps.py), and
# the alias offsets alias_check.py compiles (test_alias_check.py), and
# the file merges of merge_files.py (test_merge_files.py), and the
# declaration moves of decls.py (test_decls.py), and emu.py's scripts,
# steps, recordings and key bindings (test_emu.py), and the LZ77, tile and
# PNG code of gfx.py (test_gfx.py), and the encoding and layout of
# text.py (test_text.py);
# no ROM or build needed
# (test_ds.py, test_pragmas.py, test_absolute.py, test_data_order.py,
# test_bss.py, test_data.py and test_alias_check.py use the compiler from make setup; test_ds.py
# checks more when extract/ exists, test_bss.py when build/fnindex.json and
# build/dsindex.json exist, test_data.py when the ROM exists as well;
# test_sound.py checks the round trip of the ROM's archive when extract/ exists,
# test_gfx.py that of the /syogun pictures, test_text.py that of the text
# banks and the order of its C with the compiler;
# test_gdb_rsp.py and test_emu.py never start melonDS).
# Git exports GIT_DIR, GIT_INDEX_FILE and others to hooks, so a test run from
# pre-commit would send the git commands meant for a throwaway repo to the
# repository being committed (issue 0076). TEST_ENV drops every GIT_ variable
# from the environment except GIT_EDITOR.
TEST_ENV = env $(foreach v,$(filter-out GIT_EDITOR,$(filter GIT_%,$(.VARIABLES))),$(if $(filter environment%,$(origin $(v))),-u $(v)))
# Tests that read nothing from the checkout but tools/, the Makefile,
# tools.lock and docs/style/ (and the tools from tools.lock). test_landing.py
# takes most of the test time.
CACHED_TESTS := tools/tests/test_landing.py
TESTS := \
	tools/tests/test_propagate.py \
	tools/tests/test_fn_rename.py \
	tools/tests/test_loop_state.py \
	tools/tests/test_fnshow.py \
	tools/tests/test_fn_search.py \
	tools/tests/test_fnadd.py \
	tools/tests/test_ds.py \
	tools/tests/test_pragmas.py \
	tools/tests/test_protocheck.py \
	tools/tests/test_permuter.py \
	tools/tests/test_absolute.py \
	tools/tests/test_data_inc.py \
	tools/tests/test_diff_rows.py \
	tools/tests/test_align2.py \
	tools/tests/test_arm7.py \
	tools/tests/test_ranking.py \
	tools/tests/test_portable_c.py \
	tools/tests/test_progress.py \
	tools/tests/test_data_order.py \
	tools/tests/test_bss.py \
	tools/tests/test_release_cache.py \
	tools/tests/test_data.py \
	tools/tests/test_sound.py \
	tools/tests/test_shift.py \
	tools/tests/test_scan_pointers.py \
	tools/tests/test_shift_gaps.py \
	tools/tests/test_merge_files.py \
	tools/tests/test_decls.py \
	tools/tests/test_alias_check.py \
	tools/tests/test_gdb_rsp.py \
	tools/tests/test_emu.py \
	tools/tests/test_maps.py \
	tools/tests/test_savedit.py \
	tools/tests/test_gfx.py \
	tools/tests/test_text.py

test:
	@for t in $(CACHED_TESTS) $(TESTS); do $(TEST_ENV) $(PYTHON) $$t || exit 1; done

# make release runs this instead of test: CACHED_TESTS are skipped when they
# passed with the same tools/, Makefile, tools.lock and docs/style/
# (tools/release_cache.py test; RELEASE_CACHE=off runs them).
test-release:
	@$(PYTHON) tools/release_cache.py test $(CACHED_TESTS)
	@for t in $(TESTS); do $(TEST_ENV) $(PYTHON) $$t || exit 1; done

clean:
	rm -rf build build.ninja .ninja_log .ninja_deps
