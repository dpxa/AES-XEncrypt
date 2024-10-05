# removes contents from existing build if build folder exists
if (Test-Path -Path ".\build") {
    Remove-Item -Recurse -Force -Path ".\build"
}

# source files in this directory build files in build subdirectory
cmake -S . -B build

# build project
cmake --build build