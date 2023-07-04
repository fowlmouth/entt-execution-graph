FROM debian:12 AS build

RUN apt-get update \
  && apt-get install -y \
    build-essential pkg-config git graphviz curl cmake \
    libbox2d-dev libsfml-dev

WORKDIR /build

ADD Makefile ./

RUN make download -j"$(nproc)"

ADD src ./src

RUN make -j"$(nproc)" graph.png


FROM debian:12 AS final

RUN apt-get update \
  && apt-get install -y \
    libbox2d2 libsfml-graphics2.5

WORKDIR /app

COPY --from=build /build/graph.png ./
COPY --from=build /build/entt-test ./

ENTRYPOINT ["./entt-test"]
CMD []
