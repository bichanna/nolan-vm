BUILD_DIR := "builddir"

default: build

configure:
  meson setup --buildtype=debug {{BUILD_DIR}}

build: configure
  meson compile -C {{BUILD_DIR}}

test: build
  meson test -C {{BUILD_DIR}}

install: build
  meson install -C {{BUILD_DIR}}

release:
  meson setup --buildtype=release {{BUILD_DIR}}
  meson compile -C {{BUILD_DIR}}

format:
  clang-format -i ./**/*.c
  clang-format -i ./**/**/*.c
  clang-format -i ./**/*.h
  clang-format -i ./**/**/*.h

clean:
  rm -rf {{BUILD_DIR}}
