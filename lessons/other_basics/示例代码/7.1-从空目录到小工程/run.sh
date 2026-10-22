#!/usr/bin/env bash
set -eu

cd "$(dirname "$0")"
python3 compute.py camera.json
