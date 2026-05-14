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
### 1. Allow Docker to access display

```bash
xhost +local:docker
```


## Build and Run (Local — With Audio)

From the root of the repo:

### 1. Configure CMake
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/GradeYourMelody
```

# 🔊 Audio Setup (WSL + Docker)
This guide explains how to get **audio working in WSL (Windows Subsystem for Linux)** using **WSLg**.
---
## 🧠 Background

In WSL (Windows 11), audio is handled by **WSLg**, which exposes a PulseAudio server at:

/mnt/wslg/PulseServer

---
## ✅ Step 1 — Verify PulseAudio is running in WSL
Run:
```bash
ls -l /mnt/wslg/PulseServer
```
Expected: You should see a socket file (not "No such file or directory").
---
## ✅ Step 2 — Check PulseAudio connection
Run:
```bash
echo $PULSE_SERVER
pactl info
```
Expected output should include:
```
Server String: unix:/mnt/wslg/PulseServer
Server Name: pulseaudio
```
---
## ⚠️ Step 3 — Fix PulseAudio cookie issue (if needed)
If you see:
```
Failed to read cookie file ... Is a directory
```
That means your cookie path is broken.
**Fix it:**

```bash
rm -rf ~/.config/pulse/cookie

```
*(Optional)* Recreate directory:
```bash
mkdir -p ~/.config/pulse
```
> ⚠️ In WSLg, you do **NOT** need to manually manage the cookie — it is handled automatically.
---
## 🐳 Step 4 — Test PulseAudio inside Docker
Run:

```bash

docker run --rm \
  -e PULSE_SERVER=unix:/mnt/wslg/PulseServer \
  -v /mnt/wslg:/mnt/wslg \
  grade-your-melody pactl info
```

Expected:
- Same PulseAudio info output as WSL
- No "connection refused"
---
## 🚀 Step 5 — Run the app with audio
```bash
docker run --rm \
  -e DISPLAY=$DISPLAY \
  -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
  -e XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir \
  -e PULSE_SERVER=unix:/mnt/wslg/PulseServer \
  -v /mnt/wslg:/mnt/wslg \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  grade-your-melody ./build/GradeYourMelody
```
---
```
If you don't mount `/mnt/wslg`, audio will not work.

---
## ✏️ Quick Test

After launching the app:

- Place a note → you should hear preview audio
- Click Play → full sequence should play

---

## 🧾 Summary

| Requirement | Value |
|-------------|-------|
| Pulse server | `/mnt/wslg/PulseServer` |
| Mount needed | `/mnt/wslg:/mnt/wslg` |
| Works in WSL | ✅ Yes |

```

## Run with Audio in Docker (PulseAudio) for UBUNTU 22.04

> ⚠️ This setup allows audio playback from inside the Docker container using PulseAudio.
> Requires a Linux host with PulseAudio or PipeWire (Pulse compatibility).
---

### 1. Allow Docker to access display

```bash
xhost +local:docker
```
### 2. For Linux Ubuntu: Run app with Audio enabled
```bash
docker run --rm \
  --user $(id -u):$(id -g) \
  -e DISPLAY=$DISPLAY \
  -e XDG_RUNTIME_DIR=/run/user/$(id -u) \
  -e PULSE_SERVER=unix:/run/user/$(id -u)/pulse/native \
  -e PULSE_COOKIE=/home/$(whoami)/.config/pulse/cookie \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /run/user/$(id -u)/pulse:/run/user/$(id -u)/pulse \
  -v ~/.config/pulse/cookie:/home/$(whoami)/.config/pulse/cookie:ro \
  --device /dev/snd \
  grade-your-melody ./build/GradeYourMelody

3. (Optional) Test Audio Connection

You can verify PulseAudio connectivity inside Docker:

docker run --rm \
  --user $(id -u):$(id -g) \
  -e XDG_RUNTIME_DIR=/run/user/$(id -u) \
  -e PULSE_SERVER=unix:/run/user/$(id -u)/pulse/native \
  -e PULSE_COOKIE=/home/$(whoami)/.config/pulse/cookie \
  -v /run/user/$(id -u)/pulse:/run/user/$(id -u)/pulse \
  -v ~/.config/pulse/cookie:/home/$(whoami)/.config/pulse/cookie:ro \
  --device /dev/snd \
  grade-your-melody pactl info

If successful, it will print PulseAudio server information.

### Ubuntu (VM) — GUI only (no audio)
```bash
xhost +local:docker
docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix grade-your-melody ./build/GradeYourMelody
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

