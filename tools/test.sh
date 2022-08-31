#!/usr/bin/env sh

tested=0
passing=0
failing=0

for t in "$@"; do
	name=$(basename "$t")
	echo "[BEGIN]  : $name"
	./"$t"
	ec=$?
	if [ $ec -eq 0 ]; then
		echo "[OK]     : status=$ec"
		passing=$((passing+1))
	else
		echo "[FAIL]   : status=$ec"
		failing=$((failing+1))
	fi
	tested=$((tested+1))
done

echo "[SUMMARY]: tested=$tested passing=$passing failing=$failing"
exit $failing
