#!/bin/python
import os
import subprocess

binaryDir = os.getcwd()
sourceDir = os.path.dirname(os.path.realpath(__file__)) + "/"

sources = [
    'image-file-writer.cppm',
    "../common/error.cpp",

    "render-view.cpp",
    "render-stats.cpp",
    "image.cpp",
    "image-drawing-simple.cpp",
    ]

includeDirectories = [
    ".",
    "..",
    "../external/",
    "../external/imgui",
    "../app-kit/",
    "../gui-kit/"
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

cxxFlags = ["-fPIC", "-std=gnu++17", "-stdlib=libstdc++", "-fmodules-ts", "-MD", "-MT", "-lstdc++"]
linkFlags = ["-shared"]

objectFiles = []

def compileModuleFile(path, objectFiles):
    baseName = os.path.splitext( os.path.basename(sourceFile) )[0]
    outputFile = baseName + ".pcm"
    cmd = ["clang"] + cxxFlags + [prebuildModulePath, "--precompile", "-x", "c++-module"] + expandedIncludeDirectories + [sourceDir + sourceFile, "-o", baseName + ".pcm"]
    
    objectFiles.append(outputFile)
    
    print (" ".join(cmd))
    result = subprocess.run(cmd)
    return result.returncode

def compileCppFile(path, objectFiles):
    baseName = os.path.splitext( os.path.basename(sourceFile) )[0]
    outputFile = baseName + ".o"
    cmd = ["clang"] + cxxFlags + [prebuildModulePath, "-c"] + expandedIncludeDirectories + [sourceDir + sourceFile, "-o", outputFile]

    objectFiles.append(outputFile)

    print (" ".join(cmd))
    result = subprocess.run(cmd)
    return result.returncode

def compileSourceFile(path, objectFiles):
    splitPath = os.path.splitext( os.path.basename(sourceFile) )
    baseName = splitPath[0]
    ext = splitPath[1]

    print(ext)

    if ext == ".cpp":
        return compileCppFile(path, objectFiles)

    if ext == ".cppm":
        return compileModuleFile(path, objectFiles)

def linkShared(objectFiles, output):
    cmd = ["clang"] + cxxFlags + linkFlags + [prebuildModulePath] + expandedIncludeDirectories + objectFiles + ["-o", output + ".so"]

    print (" ".join(cmd))
    result = subprocess.run(cmd)
    return result.returncode

for sourceFile in sources:
    compileSourceFile(sourceFile, objectFiles)

linkShared(objectFiles, "katla-graphics")

print(objectFiles)

exit(0)
