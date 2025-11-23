#!/usr/bin/env bash
# Roda os testes de fail da etapa 5.
# Uso: ./run_fail_tests.sh [EXECUTAVEL] [PASTA_FAIL]
# Ex.: ./run_fail_tests.sh ./etapa5 test/fail

set -euo pipefail

EXEC="${1:-./etapa5}"
FAIL_DIR="${2:-test/fail}"

if [[ ! -x "$EXEC" ]]; then
  echo "Executable not found or not executable: $EXEC" >&2
  exit 2
fi

if [[ ! -d "$FAIL_DIR" ]]; then
  echo "Fail tests directory not found: $FAIL_DIR" >&2
  exit 2
fi

expected_code_for_macro() {
  case "$1" in
    ERR_UNDECLARED)      echo 10 ;;
    ERR_DECLARED)        echo 11 ;;
    ERR_VARIABLE)        echo 20 ;;
    ERR_FUNCTION)        echo 21 ;;
    ERR_WRONG_TYPE)      echo 30 ;;
    ERR_MISSING_ARGS)    echo 40 ;;
    ERR_EXCESS_ARGS)     echo 41 ;;
    ERR_WRONG_TYPE_ARGS) echo 42 ;;
    *) echo "" ;;
  esac
}

shopt -s nullglob
tests=( "$FAIL_DIR"/fail* )
if (( ${#tests[@]} == 0 )); then
  echo "No fail tests found in $FAIL_DIR" >&2
  exit 2
fi

pass=0
fail=0

for t in "${tests[@]}"; do
  base="$(basename "$t")"
  first_line="$(head -n1 "$t" | tr -d '\r')"

  if [[ "$first_line" =~ ^//([A-Za-z0-9_]+) ]]; then
    macro="${BASH_REMATCH[1]}"
  else
    echo "[$base] FAIL (faltou //ERR_* na primeira linha)"
    ((fail++))
    continue
  fi

  expected="$(expected_code_for_macro "$macro")"
  if [[ -z "$expected" ]]; then
    echo "[$base] FAIL (macro desconhecido '$macro')"
    ((fail++))
    continue
  fi

  out="$(mktemp)"
  err="$(mktemp)"

  set +e
  "$EXEC" < "$t" >"$out" 2>"$err"
  status=$?
  set -e

  if [[ $status -eq $expected ]]; then
    echo "[$base] PASS ($macro)"
    ((pass++))
  else
    echo "[$base] FAIL esperado $macro($expected), veio exit $status"
    if [[ -s "$err" ]]; then
      echo "stderr:"
      sed 's/^/  /' "$err" | head -n 40
    fi
    ((fail++))
  fi

  rm -f "$out" "$err"
done

echo "----"
echo "Pass: $pass  Fail: $fail  Total: $((pass+fail))"

if (( fail > 0 )); then
  exit 1
fi
exit 0
