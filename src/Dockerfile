FROM debian:bookworm-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    pkg-config \
    libarchive-dev \
    libcrypt-dev \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd -g 1000 default \
    && useradd -u 1000 -g default -d /home/default -s /bin/bash -m default

WORKDIR /app

COPY . .


RUN cmake -S . -B build \
    && cmake --build build

EXPOSE 80

USER default

CMD ["./build/workspace-controller"]

