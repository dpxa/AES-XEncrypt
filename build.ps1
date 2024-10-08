# removes contents from existing build if build folder exists
if (Test-Path -Path ".\build") {
    Remove-Item -Recurse -Force -Path ".\build"
}

if (Test-Path "dir-enc.exe") {
    Remove-Item -Force -Path "dir-enc.exe"
}

# the source files are in this directory
# the build files go in build subdirectory
cmake -S . -B build

# build project
cmake --build build

# move generated executable to current directory
Move-Item -Path "build\dir-enc.exe" -Destination "dir-enc.exe"