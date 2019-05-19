#!/bin/python
import os
import subprocess

binaryDir = os.getcwd()
sourceDir = os.path.dirname(os.path.realpath(__file__)) + "/"

sources = [
    "../common/error.cpp",
    "text-rendering-freetype.cpp",
    "font-face-freetype.cpp",
    "font-face-surface-writer.cpp",
    "font-face-cache.cpp"]

includeDirectories = [
    ".",
    "..",
    "../external/",
    "../external/imgui",
    "../app-kit/",
    "../gui-kit/",
    "../../graphics/",

    "/usr/include/freetype2",
    "/usr/include/libpng16",
    "/usr/include/harfbuzz",
    "/usr/include/glib-2.0",
    "/usr/lib/glib-2.0/include"
]

modules = [
    "image-file-writer.pcm"
]

expandedIncludeDirectories = []
for includeDir in includeDirectories:
    if includeDir[0] == '.':
        print ('1')
        expandedIncludeDirectories = expandedIncludeDirectories + ["-I" + sourceDir + includeDir]
    else:
        print ('2')
        expandedIncludeDirectories = expandedIncludeDirectories + ["-I" + includeDir]

print(expandedIncludeDirectories)

prebuildModulePath = "-fprebuilt-module-path=" + binaryDir

cxxFlags = ["-fPIC", "-std=c++17", "-stdlib=libstdc++", "-fmodules-ts", "-MD", "-MT"]

objects = []

for sourceFile in sources:
    baseName = os.path.splitext( os.path.basename(sourceFile) )[0]
    
    cmd1 = ["clang"] + cxxFlags + ["-c", prebuildModulePath, "-fmodule-file=/home/simon/projects/katla/build/graphics/image-file-writer.pcm", "-latomic"] + expandedIncludeDirectories + [sourceDir + sourceFile, "-o", baseName + ".o"]
    print (" ".join(cmd1))
    subprocess.run(cmd1)

    #cmd1 = ["clang", cxxFlags, prebuildModulePath] + expandedIncludeDirectories + [sourceDir + sourceFile, "-o", baseName + ".pcm"]
    #print (" ".join(cmd1))
    #subprocess.run(cmd1)

    objects = objects + [(baseName + ".o")]

print (objects)
print ("")

exit(-1)
