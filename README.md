# Grade Your Melody

C++17 / Qt5 desktop app for placing and grading musical notes on a staff.

## Prerequisites

- Docker with the `gym-build` image (see `docker/Dockerfile`)
- An X11 display server running on the host

Build the Docker image once:

```
docker build -t gym-build docker/
```

## Build and Run

Use the provided script from the project root:

```
./scripts/run_gui.sh
```

This script:
1. Builds the project inside the `gym-build` container
2. Grants the container access to the host X server
3. Runs the GUI inside the container, displaying it on the host via X11 forwarding

## Manual Steps

Build only:

```
docker run --rm -v "$(pwd)":/work gym-build bash -c "cd /work/build && cmake .. -G Ninja && ninja"
```

Run only (after building):

```
xhost +local:docker
docker run --rm \
  -v "$(pwd)":/work \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  gym-build /work/build/app/gym_app
```
