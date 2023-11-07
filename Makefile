NPROCS ?= $(shell nproc)

# Debug cmake configuration
build/Makefile:
	@mkdir -p build
	@cd build && \
      cmake -DCMAKE_BUILD_TYPE=Debug ..

# Run cmake
.PHONY: cmake
cmake: build/Makefile

# Build using cmake
.PHONY: build
build: cmake
	@cmake --build build -j $(NPROCS)

# Run after build
.PHONY: run
run: build
	./build/ObjectCollisions

# Cleanup data
.PHONY: dist-clean
dist-clean:
	@rm -rf build

# Format the sources
.PHONY: format
format:
	@find core -name '*pp' -type f | xargs clang-format -i
