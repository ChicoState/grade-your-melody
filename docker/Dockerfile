FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
  build-essential cmake ninja-build git pkg-config \
  qtbase5-dev qtbase5-dev-tools qtchooser qt5-qmake \
  libx11-6 libx11-xcb1 libxext6 libxrender1 libxi6 libxrandr2 libxfixes3 \
  libgl1-mesa-glx libgl1-mesa-dri mesa-utils \
  x11-apps \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /work
