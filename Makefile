# Every target goes through here (D2). `make release` is the first command
# to run after cloning: it runs setup, builds and checks.

ROM      := orig/AWRE.nds
ROM_SHA1 := config/AWRE/rom.sha1
PYTHON   ?= python3
TOOLS    := .cache/tools
DSD      := $(TOOLS)/dsd/dsd
OBJDIFF  := $(TOOLS)/objdiff-cli/objdiff-cli
SKREW    := $(TOOLS)/metroskrew

.PHONY: all setup release check check-rom hooks clean

all: check-rom

# Downloads tools.lock entries into .cache/ once; offline after that.
setup: hooks
	@$(PYTHON) tools/setup.py

hooks:
	@if [ "$$(git config --get core.hooksPath)" != hooks ]; then \
		git config core.hooksPath hooks && echo "setup: git hooks installed (core.hooksPath=hooks)"; \
	fi

check-rom:
	@if [ ! -e $(ROM) ]; then \
		echo "error: $(ROM) not found. Link your dump there, for example:" >&2; \
		echo "  mkdir -p orig && ln -s /path/to/your/awds.nds $(ROM)" >&2; \
		exit 1; \
	fi
	@sha1sum --quiet -c $(ROM_SHA1) || { echo "error: $(ROM) is not the US/AUS AWRE retail ROM" >&2; exit 1; }

check: check-rom

release: setup
	@$(MAKE) --no-print-directory all
	@$(MAKE) --no-print-directory check
	@echo "release: OK"

clean:
	rm -rf build
