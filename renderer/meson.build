project('skia-test', 'cpp', default_options : ['cpp_std=c++17'])

# for now set these manually
skia_build_path = '../../../../skia/out/Shared/'

glfw3 = dependency('glfw3', version : '>=3.2.0')
gl = dependency('gl', version : '>=17.0.0')

skia_include_dirs = include_directories(
    '../../../skia/include',
        '../../../skia/include/core',
        '../../../skia/include/config',
        '../../../skia/include/gpu',
        '../../../skia/include/utils',
        '../../../skia/include/private'        
    )

# the -Wl rpath + dot options are needed to set the rpath, so it can find the skia lib (copy libskia.so to your build dir). temporary, just for testing only.
skia_link_libdir = '-L' + skia_build_path
skia_rpath_libdir = '-Wl,' + skia_build_path
skia = declare_dependency(
    link_args: ['-lskia', skia_link_libdir, '-Wl,-rpath', skia_rpath_libdir],
    include_directories: skia_include_dirs
    )
# 

cpp = [
    'main.cpp',
    'common/error.cpp',
    'drawing/skia-opengl.cpp',
    'gpu/vulkan/vulkan.cpp',
    'gpu/vulkan/vulkan-window.cpp',
    'gpu/vulkan/vulkan-window-factory.cpp',
    'gpu/opengl/opengl.cpp',
    'gpu/opengl/opengl-window.cpp',
    'gpu/opengl/opengl-window-factory.cpp'
]

executable('skia-test', cpp, dependencies: [glfw3, gl, skia])