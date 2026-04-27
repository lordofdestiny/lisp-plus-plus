run: build_and_format
	@build/main

build_and_format: format
	@make build/main
	
build/main: main.cpp | build
	@clang++ -std=c++20 -o $@ $< -Iinclude -include all.hpp -MD -MF build/main.d -E

build:
	@mkdir -p build

clean:
	@rm -rf main
	@rm -rf build

format:
	@clang-format -files=fmt.txt -i > /dev/null 2>&1

.PHONY: all clean format build

-include build/main.d
