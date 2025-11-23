#!/usr/bin/env bash
set -euo pipefail

# Uso:
#   ./run_pass_tests.sh ./seu_executavel [diretorio_testes]
#
# Defaults:
EXEC="${1:-./etapa5}"        # ajuste o default se teu binário tiver outro nome
TEST_DIR="${2:-test/pass}"

if [[ ! -x "$EXEC" ]]; then
  echo "Erro: executável '$EXEC' não existe ou não é executável."
  echo "Uso: $0 ./path/do/binario [diretorio_testes]"
  exit 2
fi

if [[ ! -d "$TEST_DIR" ]]; then
  echo "Erro: diretório de testes '$TEST_DIR' não existe."
  exit 2
fi

passed=0
failed=0
total=0

# garante ordem pass00..pass99
for input in "$TEST_DIR"/pass[0-9][0-9]; do
  [[ -f "$input" ]] || continue
  base="$(basename "$input")"
  expected="$TEST_DIR/$base.iloc"
  total=$((total + 1))

  if [[ ! -f "$expected" ]]; then
    echo "[WARN] esperado não encontrado: $expected (pulando)"
    continue
  fi

  out="$(mktemp)"
  err="$(mktemp)"

  # roda
  if ! "$EXEC" < "$input" > "$out" 2> "$err"; then
    echo "[$base] FAIL (executável retornou erro)"
    echo "stderr:"
    sed 's/^/  /' "$err"
    failed=$((failed + 1))
    rm -f "$out" "$err"
    continue
  fi

  # compara (strip-trailing-cr ajuda se tiver CRLF)
  if diff -u --strip-trailing-cr "$expected" "$out" > /dev/null; then
    echo "[$base] OK"
    passed=$((passed + 1))
  else
    echo "[$base] FAIL (diff abaixo)"
    diff -u --strip-trailing-cr "$expected" "$out" | sed 's/^/  /'
    failed=$((failed + 1))
  fi

  rm -f "$out" "$err"
done

echo
echo "Resumo: $passed passaram, $failed falharam, $total no total."

if [[ $failed -ne 0 ]]; then
  exit 1
fi
