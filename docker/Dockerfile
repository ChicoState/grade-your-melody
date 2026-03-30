FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-qmltooling-plugins \
    qt6-wayland \
    libgl1-mesa-dev \
    qml6-module-qtquick \
    qml6-module-qtqml \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-templates \
    qml6-module-qtquick-layouts \
    qml6-module-qtqml-workerscript \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

# Build
RUN rm -rf build \
 && cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build
