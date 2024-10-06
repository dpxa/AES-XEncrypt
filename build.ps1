# removes contents from existing build if build folder exists
if (Test-Path -Path ".\build") {
    Remove-Item -Recurse -Force -Path ".\build"
}

# the source files are in this directory
# the build files go in build subdirectory
cmake -S . -B build

# build project
cmake --build build

mv build\dir-enc.exe dir-enc.exe