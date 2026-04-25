run: build_and_format
	@./main

build_and_format: format
	@make main
	
main: main.cpp
	@g++ -std=c++20 -o $@ $< -Iinclude -include all.hpp

clean:
	@rm -rf main
	@rm -rf build

format:
	@clang-format -files=fmt.txt -i

.PHONY: all clean format