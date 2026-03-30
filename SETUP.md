# Grade Your Melody - Setup Guide

## Prerequisites
- Docker installed
- For GUI display (Ubuntu/WSL): X11 server running

---

## Build the Docker Image

From the **root of the repo** (`grade-your-melody/`):

```bash
docker build -t grade-your-melody -f docker/Dockerfile .
```

---

## Run the App

### Ubuntu (VM)
```bash
xhost +local:docker
docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix grade-your-melody ./build/GradeYourMelody
```

### WSL (Windows)
1. Install **VcXsrv** or **X410** on Windows and start it
2. Then run:
```bash
docker run -e DISPLAY=host.docker.internal:0 grade-your-melody ./build/GradeYourMelody
```

### Mac
1. Install **XQuartz** and enable "Allow connections from network clients" in settings
2. Then run:
```bash
docker run -e DISPLAY=host.docker.internal:0 grade-your-melody ./build/GradeYourMelody
```

---

## Notes
- The `DEPRECATED` warning about the legacy builder is harmless, ignore it
- If you get a cmake cache error, make sure you're using the latest Dockerfile which includes `rm -rf build` before the cmake step
- If the app window doesn't open, make sure you ran `xhost +local:docker` first (Ubuntu only)
