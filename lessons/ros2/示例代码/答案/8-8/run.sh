#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

if timeout 60 launch_test "$(dirname "$0")/test_subscriber.launch.py" >"$tmp/test.log" 2>&1; then
  sed -n 's/^Ran 1 test.*/Ran 1 test/p; /^OK$/p' "$tmp/test.log" |
    awk '!seen[$0]++'
else
  cat "$tmp/test.log"
  exit 1
fi
