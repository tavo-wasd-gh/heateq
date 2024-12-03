BIN = heateq

GO = go
HUGO = hugo

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Wpedantic -fopenmp
LXX = g++
LXXFLAGS = -fopenmp -lpthread

CPPSRC = \
	 src/cpp/heatmap.cpp \
	 src/cpp/main.cpp \

CPPHEAD = \
	  src/cpp/heatmap.hpp \

CPPOBJ = $(CPPSRC:.cpp=.o)

all: $(BIN)

$(BIN): $(CPPOBJ)
	$(LXX) $(LXXFLAGS) -o $@ $^

src/%.o: src/%.cpp $(CPPHEAD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

serve: .deps-lock
	$(HUGO) server \
		--disableFastRender \
		--noHTTPCache \
		--ignoreCache

docs: .public-lock
.public-lock: .deps-lock config.toml $(shell find content -type f)
	$(HUGO) --gc --minify
	@touch .public-lock

.deps-lock:
	@$(GO) version > /dev/null
	@$(HUGO) version > /dev/null
	$(GO) mod init github.com/tavo-wasd-gh/heateq > /dev/null
	$(HUGO) mod get -u > /dev/null
	@touch .deps-lock

clean:
	rm -rf $(CPPOBJ) $(BIN)

clean-all: clean
	rm -rf \
		public/ \
		resources/ \
		go.mod go.sum \
		.hugo_build.lock \
		.deps-lock \
		.public-lock

.PHONY: all docs serve clean
