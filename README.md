# katla
![Docker Image CI](https://github.com/plok/katla/workflows/Docker%20Image%20CI/badge.svg)

## Renderer test 

Dependencies (Arch Linux):
- pacman -S vulkan-devel [glfw-x11 | glfw-wayland] glm 

For building shaders in arch: glslang

# build with skia

Dependencies
- gawk (only for skia build script)

Setup python to use python2
$ pacman -S python2-virtualenv

$ cd <your-workspace>

$ virtualenv2 -p /usr/bin/python2 --distribute venv-py2  
$ source venv-py2/bin/activate  

Clone & setup repo-tools:
$ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git'  
$ export PATH="${PWD}/depot_tools:${PATH}"

Setup Build Skia
$ git clone https://github.com/plok/katla.git  
$ cd katla/external/skia  
$ python tools/git-sync-deps  

Build Skia

- add <unicode/unistr.h> include to "third_part/externals/sfntly/cpp/src/sample/chromium/subsetter_impl.cc"

$ ./bin/gn gen out/Shared --args='is_official_build=true is_component_build=true skia_use_icu=false skia_use_sfntly=false extra_cflags_cc=["-Wno-error"]'
$ cd out/Shared
$ ninja
