#!/usr/bin/env bash
set -euo pipefail
ANSWER_WS="$(cd "$(dirname "$0")" && pwd)/ws"
source "$(dirname "$0")/../lib.sh"

ros2 run answer_nodes circle_server >/dev/null 2>&1 &
sleep 1
ros2 run answer_nodes circle_client
