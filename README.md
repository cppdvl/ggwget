# ggwget

`ggwget` is a minimal `wget`-like utility implemented in C++20. It provides a reusable download library plus a simple CLI example, both linked statically against libcurl fetched via CMake's `FetchContent`.

## Build

1. Ensure CMake ≥ 3.21, a C++20 compiler, and Git are available.
2. Choose an installation prefix (required) and configure the project. The first configure clones and builds libcurl as a static library:
   ```sh
   cmake -S . -B build -DGGWGET_INSTALL_PREFIX=/absolute/path/to/prefix
   ```
3. Build the library and example (targets `ggwget` and `ggwget_example`):
   ```sh
   cmake --build build --target ggwget ggwget_example
   ```
4. The resulting static library lives at `build/libggwget.a`, and the CLI example binary at `build/ggwget_example`.

## Usage

`ggwget_example` streams downloaded content to stdout by default. Use the `-o` flag to write to a file instead.

```sh
# Stream a web page directly to stdout
./build/ggwget_example https://example.com

# Download to a file
./build/ggwget_example -o page.html https://example.com
```

Run `./build/ggwget_example --help` for the built-in usage summary.

## Install

Installation always targets the prefix supplied during configuration. Trigger it with:

```sh
cmake --build build --target install_ggwget
```

This installs the static library to `<prefix>/lib`, headers to `<prefix>/include`, CMake package files to `<prefix>/lib/cmake/ggwget`, and the CLI example to `<prefix>/bin`.
