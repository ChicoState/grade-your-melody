#!/usr/bin/env bash
set -e

# resolve project root regardless of where the script is called from
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# build inside the container
docker run --rm \
  -v "$PROJECT_ROOT":/work \
  gym-build \
  bash -c "cd /work/build && cmake .. -G Ninja && ninja"

# allow local Docker containers to connect to the host X server
xhost +local:docker

# run the GUI inside the container with X11 forwarding
docker run --rm \
  -v "$PROJECT_ROOT":/work \
  -e DISPLAY="$DISPLAY" \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  gym-build \
  /work/build/app/gym_app
