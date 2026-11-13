
.PHONY: build
build:
	cmake --preset linux-debug
	cmake --build --preset linux-debug

.PHONY: run
run: build
	LD_LIBRARY_PATH="$(CURDIR)/lib$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}" \
		./build/linux-debug/examples/amplitude-set-get/amplitude-set-get

.PHONY: run_py
run_py:
	LD_LIBRARY_PATH="$(CURDIR)/lib$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}" \
		python3 examples/amplitude-set-get-py/main.py

.PHONY: clean
clean:
	if [ -f build/linux-debug/CMakeCache.txt ]; then cmake --build build/linux-debug --target clean; fi
