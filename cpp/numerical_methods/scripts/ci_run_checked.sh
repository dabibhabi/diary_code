#!/usr/bin/env bash
# Runs only the ctest labels matching checked TODO.md items (plus baseline).
# Used by the GitHub Actions cpp/python jobs and by `make test`.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-${ROOT}/build}"

if [ ! -d "${BUILD_DIR}" ]; then
    echo "[ci] build directory missing: ${BUILD_DIR}" >&2
    exit 1
fi

LABEL_REGEX="$(python3 "${ROOT}/scripts/parse_todo.py" "${ROOT}/TODO.md")"
echo "[ci] running ctest with -L '${LABEL_REGEX}'"

exec ctest --test-dir "${BUILD_DIR}" -L "${LABEL_REGEX}" --output-on-failure --no-tests=ignore
