#!/usr/bin/env sh

# run from repo root: `make test`
# every examples/X.sel needs examples/X.exp (expected exit code, or "compile-error")

APP=build/sel
T=build/tests
mkdir -p $T
pass=0; fail=0
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'
echo

for sel in examples/*.sel; do
  name=$(basename "$sel" .sel)
  exp="${sel%.sel}.exp"
  if [ ! -f "$exp" ]; then
    echo "FAIL $name (missing $exp)"; fail=$((fail+1)); continue
  fi

  # compile -> assemble -> link -> run
  # any failing step before the run = compile-error
  if $APP "$sel" $T/$name.s 2>/dev/null && as $T/$name.s -o $T/$name.o && ld $T/$name.o -o $T/$name; then
    $T/$name; got=$?
  else
    got=compile-error
  fi

  want=$(cat "$exp")
  if [ "$got" = "$want" ]; then
    printf " ${GREEN}✓    %s${RESET}\n" "$name"; pass=$((pass+1))
  else
    printf " ${RED}✗    %s (want %s, got %s)${RESET}\n" "$name" "$want" "$got"
  fi
done

echo
printf " $pass passed"
echo
printf " $fail failed"
echo
[ "$fail" -eq 0 ]
