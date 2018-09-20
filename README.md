# katla

Very very work in progress!!

## Renderer test 

Dependencies (Arch Linux):
- pacman -S vulkan-devel [glfw-x11 | glfw-wayland] glm


# build skia

Setup python to use python2
$ pacman -S python2-virtualenv

$ virtualenv2 -p /usr/bin/python2 --distribute venv-py2
$ source venv-py2/bin/activate

Clone & setup repo-tools:
$ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git'
$ export PATH="${PWD}/depot_tools:${PATH}
$ alias python=python2

Setup Build Skia
$ cd skia
$ python tools/git-sync-dep

Build Skia

- add <unicode/unistr.h> include to "third_part/externals/sfntly/cpp/src/sample/chromium/subsetter_impl.cc"

$ ./bin/gn gen out/Shared --args='is_official_build=true is_component_build=true extra_cflags_cc=["-Wno-error"]'
$ cd out/Shared
$ ninja
