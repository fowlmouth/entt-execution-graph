BIN := entt-test

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

CXXFLAGS := -std=c++20 -Ientt-3.11.1/src $(shell pkg-config --cflags sfml-graphics)
LDLIBS := -lbox2d $(shell pkg-config --libs sfml-graphics)

DOCKER_IMAGE := entt-tests:latest

ALL: $(BIN) graph.png
.PHONY: clean cleanall deps run

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp entt-3.11.1
	$(CXX) $(CXXFLAGS) -o $@ -c $<

download: entt-3.11.1

entt-3.11.1:
	curl --remote-name --location https://github.com/skypjack/entt/archive/refs/tags/v3.11.1.tar.gz
	tar -xzf v3.11.1.tar.gz

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ)
cleanall: clean
	rm -f $(BIN)

graph.dot: $(BIN)
	./$(BIN) --dot >graph.dot
graph.png: graph.dot
	dot -Tpng $< -o $@

image.tgz: $(SRC)
	docker build . --tag "$(DOCKER_IMAGE)"
	docker save "$(DOCKER_IMAGE)" | gzip -7 > image.tgz
