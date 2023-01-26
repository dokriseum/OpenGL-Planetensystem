## Prerequisites
- cmake
- make(Linux/Unix and MacOS) or Visual Studio(Windows)

## How to clone
Use `git clone --recursive https://github.com/dokriseum/CGTutorial.git` to clone. Otherwise, the submodules ([glew](https://github.com/Perlmint/glew-cmake), [glfw](https://github.com/glfw/glfw), [glm](https://github.com/g-truc/glm)) won't be cloned.

## How to build
### Linux/Unix and MacOS
First, you have to install some standard tools: `libgl1-mesa-dev`, `libxrandr-dev,` `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`  
Your current directory has to be `CGTutorial/`.  
1. Create the Makefile: `cmake -S . -B build`
2. Use make to build the project: `cd build && make`

### Debian, Ubuntu, Linux Mint

sudo apt install libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules xorg-dev build-essential cmake -y

You should end up with an executable `CGTutorial/build/CGTutorial`.  
MacOS could have some problems since some Apple Systems don't support OpenGL 3.*. 

### Windows (Visual Studio)
1. Open the folder `CGTutorial` in Visual Studio. It should detect the `CMakeLists.txt` and build the solution automatically in Debug mode.
2. Choose the target called `CGTutorial.exe`.
---
Inspired by https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton
