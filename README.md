# OpenGL Playground

Small opengl renderer with an ECS architecture

http://igm.univ-mlv.fr/~lnoel/index.php?section=teaching&teaching=opengl&teaching_section=tds


## Getting Started

### Prerequisites

You need to install [Cmake](https://cmake.org/) to build the project, and [Conan](https://conan.io/) to download dependencies.

Then you can add the server which contains the dependencies of the project :

```bash
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

### Build

You can handle the `CMakeLists.txt` in any way you like, it will download the dependecies by itself. 

Here's some way to use it :

#### `Option 1: CLI`

```bash
cmake . -DCMAKE_BUILD_TYPE=Release
make
```

#### `Option 2: Visual Studio (Windows only)`

Open this folder with the `CMake...` option in file->open on Visual Studio, and run the project.

#### `Option 3: VSCode`

Use the `CMakeTools` plugin, build with `f7` then run with `f5` (But be carefull to be on the right platform, there is a launch file for windows and for linux).
