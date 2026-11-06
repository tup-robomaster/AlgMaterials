#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 interface show --no-comments turtlesim/action/RotateAbsolute
