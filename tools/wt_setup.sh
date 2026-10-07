#!/bin/sh
# Prepare a git worktree of this repo for building: link the ROM and the
# tool cache from the main checkout, so nothing is downloaded or copied.
# Run from the worktree root. Safe to run twice.
set -eu

main=$(dirname "$(git rev-parse --path-format=absolute --git-common-dir)")
here=$(git rev-parse --show-toplevel)
if [ "$main" = "$here" ]; then
    echo "wt_setup: this is the main checkout, nothing to do"
    exit 0
fi
[ -e "$main/orig/AWRE.nds" ] || { echo "error: $main/orig/AWRE.nds is missing" >&2; exit 1; }
[ -d "$main/.cache/tools" ] || { echo "error: run make setup in $main first" >&2; exit 1; }

mkdir -p "$here/orig"
ln -sfn "$(readlink -f "$main/orig/AWRE.nds")" "$here/orig/AWRE.nds"
[ -e "$here/.cache" ] || ln -s "$main/.cache" "$here/.cache"
echo "wt_setup: linked orig/AWRE.nds and .cache from $main"
