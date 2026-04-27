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

Use `--no-cache` after updating the Dockerfile to force a full rebuild:
```bash
docker build --no-cache -t grade-your-melody -f docker/Dockerfile .
```

---

## Run the App

## Build and Run (Local — With Audio)

From the root of the repo:

### 1. Configure CMake
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/GradeYourMelody
```

### Ubuntu (VM) — GUI + Audio
```bash
xhost +local:docker
docker run -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    --device /dev/snd \
    grade-your-melody ./build/GradeYourMelody
```

`--device /dev/snd` passes the host audio device into the container so `QAudioSink` can play sound.

### Ubuntu (VM) — GUI only (no audio)
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

## Audio Notes

The app uses `QAudioSink` (Qt6 Multimedia) with a SoundFont piano for note playback.

**SoundFont file:** `app/soundfonts/piano.sf2` is tracked in git and copied into the build output automatically at build time. No manual step is required.

**If audio is silent inside Docker:**
1. Ensure you passed `--device /dev/snd` when running the container.
2. Check that the host has a sound device: `ls /dev/snd/`.
3. The app will still run and display correctly without audio — it logs a warning but does not crash.
4. If `/dev/snd` exists but sound still does not work, the host audio server (PulseAudio/PipeWire) may not be reachable from the container. Audio is a best-effort feature in Docker; grading and note placement work without it.

---

## Notes
- The `DEPRECATED` warning about the legacy builder is harmless, ignore it
- If you get a cmake cache error, make sure you're using the latest Dockerfile which includes `rm -rf build` before the cmake step
- If the app window doesn't open, make sure you ran `xhost +local:docker` first (Ubuntu only)


## Testing
When in the docker image, navigate to the build directory and run:
```bash
ctest --output-on-failure
```

Tests also now auto run when building the docker image and the image will fail to build if tests do not pass.

