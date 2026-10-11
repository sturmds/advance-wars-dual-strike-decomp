# Every target goes through here (D2). `make release` is the first command
# to run after cloning: it runs setup, builds and checks.

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


.PHONY: all configure fnindex dsindex progress setup release check check-rom check-modules check-out hooks extract test clean shift emu-smoke

all: check-rom configure
	@$(LOCKED_NINJA)

# Cheap, and rewrites build.ninja only when it changes.
configure:
	@$(PYTHON) tools/configure.py

# Downloads tools.lock entries into .cache/ once; offline after that.
setup: hooks
	@$(PYTHON) tools/setup.py

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

progress: all
	@$(PYTHON) tools/progress.py

release: setup
	@$(MAKE) --no-print-directory all
	@$(MAKE) --no-print-directory check
	@echo "release: OK"

# A ROM with 0x20 bytes inserted in main, in build/shift/ (issue 0012).
# Checks that every symbol after the pad moved; not part of release.
shift: all
	@flock build/.ninja.lock $(PYTHON) tools/shift_build.py

# Starts retail, our build and the shifted build in DeSmuME without a window
# and compares exit status, log and the save file the game writes. A smoke
# test only (issue 0006).
emu-smoke: shift
	@$(PYTHON) tools/emu_smoke.py $(ROM) $(OUT) build/shift/AWRE.nds

# Tests of the landing tools on throwaway repos, of propagate.py's C text
# handling, of ds.py, of pragmas.py, of absolute symbols and of ARM7's address
# moves (tools/arm7.py); no ROM or build
# needed (test_ds.py, test_pragmas.py and test_absolute.py use the compiler
# from make setup; test_ds.py checks more
# when extract/ exists).
test:
	@$(PYTHON) tools/tests/test_landing.py
	@$(PYTHON) tools/tests/test_propagate.py
	@$(PYTHON) tools/tests/test_loop_state.py
	@$(PYTHON) tools/tests/test_fnshow.py
	@$(PYTHON) tools/tests/test_ds.py
	@$(PYTHON) tools/tests/test_pragmas.py
	@$(PYTHON) tools/tests/test_protocheck.py
	@$(PYTHON) tools/tests/test_absolute.py
	@$(PYTHON) tools/tests/test_arm7.py

clean:
	rm -rf build build.ninja .ninja_log .ninja_deps
