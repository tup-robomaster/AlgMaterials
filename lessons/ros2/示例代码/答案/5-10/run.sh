#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 component types | sed 's/^ *//' | grep '^robot_nodes::TalkerComponent$'
