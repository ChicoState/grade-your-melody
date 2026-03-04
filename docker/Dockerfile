FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    git \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-base-dev-tools \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-window \
    qml6-module-qtquick-layouts \
    libgl1-mesa-dev \
    libx11-6 libx11-xcb1 libxext6 libxrender1 libxi6 libxrandr2 libxfixes3 \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /work
