#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT"

fail() {
  printf 'FAIL: %s\n' "$1" >&2
  exit 1
}

check_file() {
  [ -f "$1" ] || fail "missing expected file: $1"
}

check_file src/aetherworld.h
check_file src/aetherworld.c
check_file src/aetherworld_story.c
check_file tests/aetherworld_runtime_test.c
check_file tests/run_rpg_tests.sh
check_file docs/PROCEDURAL_INSTANCES.md

grep -q '#define AETHERWORLD_MAX_LEVEL 720' src/aetherworld.h ||
  fail 'AetherWorld max level contract changed unexpectedly'
grep -q '#define AETHERWORLD_ACT_COUNT 12' src/aetherworld.h ||
  fail 'AetherWorld act count contract changed unexpectedly'
grep -q '#define AETHERWORLD_QUEST_COUNT 50' src/aetherworld.h ||
  fail 'AetherWorld quest count contract changed unexpectedly'
grep -q 'aetherworld_validate' tests/aetherworld_runtime_test.c ||
  fail 'campaign registry validation is not exercised by runtime tests'
grep -q 'aetherworld_level_from_xp' tests/aetherworld_runtime_test.c ||
  fail 'XP-to-level behavior is not exercised by runtime tests'
grep -q 'reachability' docs/PROCEDURAL_INSTANCES.md ||
  fail 'procedural-instance contract must cover reachability'

printf 'Repository source contract checks passed.\n'
