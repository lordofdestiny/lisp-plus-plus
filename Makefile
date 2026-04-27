run: build_and_format
	@echo "Running..."
	@echo "========================================="
	@build/main

build_and_format: format | build
	@echo "Building..."
	@cmake --build build
	@ln -sf build/compile_commands.json compile_commands.json 
	
build:
	@echo "Preparing..."
	@cmake --preset main > /dev/null 2>&1

clean:
	@rm -rf build

format:
	@echo "Formatting..."
	@./format.py

.PHONY: all run clean format
