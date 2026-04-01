#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
RCC="${RCC:-$ROOT_DIR/bin/rcc}"
RISCV_CC="${RISCV_CC:-riscv64-unknown-elf-gcc}"
RISCV_ABI="${RISCV_ABI:-lp64}"
SUCCESS_DIR="$ROOT_DIR/tests/cases/success"
FAIL_DIR="$ROOT_DIR/tests/cases/fail"
TMP_DIR="${TMPDIR:-/tmp}/rcc-rv64-subset"

mkdir -p "$TMP_DIR"

if [[ ! -x "$RCC" ]]; then
  echo "missing compiler binary: $RCC" >&2
  exit 1
fi

run_success_case() {
  local src="$1"
  local base asm obj
  base="$(basename "$src" .c)"
  asm="$TMP_DIR/$base.s"
  obj="$TMP_DIR/$base.o"

  "$RCC" "$src" -S -o "$asm"
  "$RISCV_CC" -mabi="$RISCV_ABI" -c "$asm" -o "$obj"
  echo "PASS success: $base"
}

run_fail_case() {
  local src="$1"
  local base asm
  base="$(basename "$src" .c)"
  asm="$TMP_DIR/$base.s"
  rm -f "$asm"

  if "$RCC" "$src" -S -o "$asm" >/dev/null 2>&1; then
    echo "FAIL negative: $base compiled successfully" >&2
    exit 1
  fi

  if [[ -e "$asm" ]]; then
    echo "FAIL negative: $base left assembly output behind" >&2
    exit 1
  fi

  echo "PASS negative: $base"
}

for src in "$SUCCESS_DIR"/*.c; do
  run_success_case "$src"
done

for src in "$FAIL_DIR"/*.c; do
  run_fail_case "$src"
done

echo "All rv64 subset tests passed."
