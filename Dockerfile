FROM debian:12

RUN apt-get update \
  && apt-get install -y \
    build-essential pkg-config git graphviz curl cmake \
    libbox2d-dev libsfml-dev

WORKDIR /build

ADD Makefile ./

RUN make download -j"$(nproc)"

ADD src ./src

RUN make -j"$(nproc)" graph.png

