BUILD_DIR := build
BIN       := $(BUILD_DIR)/numerical_methods

# Source dirs to format. Excludes build/ (auto-generated) and cuda/ (clang-format
# chokes on .cu). C files (c/, c_api headers) included.
FORMAT_FILES := $(shell find include src tests python c main.cpp \
                    \( -name '*.cpp' -o -name '*.h' -o -name '*.c' \) \
                    -not -path '*/build/*' 2>/dev/null)

.PHONY: format format-check build run test test-all clean notebooks

# ---- format ---------------------------------------------------------------
format:
	@command -v clang-format >/dev/null 2>&1 || { \
	    echo "clang-format not installed"; exit 1; }
	@clang-format -i $(FORMAT_FILES)
	@echo "[format] reformatted $(words $(FORMAT_FILES)) files"

# Read-only check used by CI: fails (non-zero) if any file would be reformatted.
format-check:
	@command -v clang-format >/dev/null 2>&1 || { \
	    echo "clang-format not installed"; exit 1; }
	@clang-format --dry-run --Werror $(FORMAT_FILES)
	@echo "[format-check] all $(words $(FORMAT_FILES)) files pass"

# ---- build (depends on format) -------------------------------------------
build: format
	@if [ ! -d $(BUILD_DIR) ]; then \
	    echo "[build] configuring (no build dir yet)"; \
	    cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release; \
	fi
	@cmake --build $(BUILD_DIR) --parallel

# ---- run (depends on binary, builds first if missing) --------------------
run:
	@if [ -x $(BIN) ]; then \
	    echo "[run] binary present — executing"; \
	else \
	    echo "[run] binary missing — building first"; \
	    $(MAKE) --no-print-directory build; \
	fi
	@$(BIN)

# ---- test: run only checked-off TODO items + baseline --------------------
test: build
	@scripts/ci_run_checked.sh

# ---- test-all: run every test (for personal validation) ------------------
test-all: build
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

# ---- notebooks: launch jupyter against notebooks/ ------------------------
notebooks:
	@command -v jupyter >/dev/null 2>&1 || { \
	    echo "jupyter not installed; run 'pip install jupyterlab pybind11'"; exit 1; }
	@jupyter lab notebooks/

# ---- clean (depends on format) -------------------------------------------
clean: format
	@if [ -d $(BUILD_DIR) ]; then \
	    rm -rf $(BUILD_DIR); \
	    echo "[clean] removed $(BUILD_DIR)/"; \
	else \
	    echo "[clean] no build dir — skipping"; \
	fi
