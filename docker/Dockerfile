FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  ninja-build \
  pkg-config \
  git \
  qtbase5-dev \
  qtbase5-dev-tools \
  qtchooser \
  qt5-qmake \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /work
