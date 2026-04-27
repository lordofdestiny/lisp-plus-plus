#!/usr/bin/python3 format.py

from pathlib import Path
import subprocess
import os


def collect_files(filters=["cpp", "hpp", "h"]):
    queue = [Path(".")]
    files = []

    while len(queue) > 0:
        dir = queue[0]
        queue.pop(0)

        for path in Path(dir).iterdir():
            if path.is_dir() and path.name == "build":
                continue

            if path.is_dir():
                queue.append(path)

            if path.is_file() and any(path.name.endswith(f) for f in filters):
                files.append(path)

    return [path.resolve() for path in files]


if __name__ == "__main__":
    files = collect_files()
    result = subprocess.run(["clang-format", "-style=Mozilla", "-i", *files])

    exit(result.returncode)
