#!/usr/bin/env bash
set -euo pipefail
grep -o '<depend>std_msgs</depend>' "$(dirname "$0")/package_fragment.xml"
