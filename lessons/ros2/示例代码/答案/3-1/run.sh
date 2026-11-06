#!/usr/bin/env bash
set -euo pipefail
ANSWER_WS="$(cd "$(dirname "$0")" && pwd)/ws"
source "$(dirname "$0")/../lib.sh"

ros2 interface show robot_interfaces/action/Patrol
