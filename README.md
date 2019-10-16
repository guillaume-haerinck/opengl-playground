[![Build Status](https://travis-ci.org/guillaume-haerinck/opengl-playground.svg?branch=master)](https://travis-ci.org/guillaume-haerinck/opengl-playground)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Try online](https://img.shields.io/badge/try-online-brightgreen)](http://lucasisawesome.eu/)

# OpenGL Playground

Small opengl renderer with an Entity Component System (ECS) architecture. [Check it online](http://lucasisawesome.eu/).

Supports WASM build so it can be played on any web-browser like Chrome, Firefox or Safari. Just run the content of the `www` folder on a web server.

## Table of Contents

+ [Getting started](#Getting-Started)
+ [Examples](#Examples)
    + [Basics](#Basics)
    + [Blinn-Phong shading](#Blinn-Phong-Shading)
    + [Non-Photorealistic rendering](#Non-Photorealistic-Rendering-NPR)
    + [Intermediate](#Intermediate)
    + [Physically based rendering](#Physically-based-rendering-PBR)
    + [Shadows](#Shadows)
	+ [Animations](#Animations)
	+ [Post-Processing](#Post-Processing)
    + [Advanced](#Advanced)
+ [Based on](#Based-On)

## Getting Started

### Prerequisites

You need to install [Cmake](https://cmake.org/) to build the project, and [Conan](https://conan.io/) to download dependencies.

Then you can add the server which contains the dependencies of the project :

```bash
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

### Build on desktop

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

### Build for the Web as WASM (linux only)

This project support Web Assembly, so it can run in a browser like Google Chrome or Firefox !

The build steps are the same for any platform (however it seems broken for now on Windows). Do not forget to delete `CMakeCache.txt` if there is one on the folder.

```bash
conan install ./wasm.recipe.py --build missing --install-folder wasm -pr ./wasm.profile
conan build ./wasm.recipe.py --build-folder wasm
```

You can then copy the files inside `wasm/bin` into `www` and run this folder with a simple web-server. On this exemple, we are creating a simple one with python :

```bash
cp -a wasm/bin/. www
cd www
python3 -m http.server -b 127.0.0.1
```

Then open [your local server](http://127.0.0.1:8000/) to see the project.

## Examples

### Basics

#### [Triangle](src/examples/basics/basic-triangle/)

<p align="left">
<img src="doc/img/basic/triangle.png?raw=true" alt="Triangle exemple" height="160px">
</p>

A simple triangle.

#### [Rotating cube](src/examples/basics/rotating-cube/)

<p align="left">
<img src="doc/img/basic/cube.png?raw=true" alt="Rotating cube exemple" height="160px">
</p>

A rotating colored cube. Each face is independant so that the colors can be applied without smearing.

#### [Textured primitives](src/examples/basics/textured-primitives/)

<p align="left">
<img src="doc/img/basic/textured-primitives.png?raw=true" alt="Textured primitive exemple" height="160px">
</p>

Show some procedurally-generated primitives with their UV maps.

#### [Model loading](src/examples/basics/model-loading/)

<p align="left">
<img src="doc/img/basic/model.png?raw=true" alt="Model loading exemple" height="160px">
</p>

Loads a GLTF model with its texture.

#### [WIC] [Skybox](#)

Show an environment in the background using a cubemap.

#### [WIC] [Global GUI](#)

Display a grid, some icons for the lights and a dynamic gizmo to always know the orientation of the camera.

### Blinn-Phong shading

####  [WIC] [Directional light](src/examples/blinn-phong/point-light/)

Setup a simple scene with a directional light and no material.
A menu is available to edit ambient, diffuse and specular properties.

#### [WIC] [Multiple light types](src/examples/blinn-phong/multiple-light-types/)

Uses multiple point, directional and spotlights.

#### [WIC] [Materials](src/examples/blinn-phong/materials/)

Uses different objects with different materials applied to them to compare how they react to lighting.

### Non-Photorealistic Rendering (NPR)

#### [WIC] [Gooch shading](#)

Change pixel color value from "cool to warm" based on normal direction angle from the camera.

#### [WIC] [Toon shading](#)

A rendering technique closer to traditional 2D animation.

### Intermediate

#### [WIC] [Object picking](#)

Select objects on the scene with the mouse and raytracing.

#### [WIC] [View frustrum culling](#)

Do not render objects outside of the camera view.

#### [WIC] [Occlusion culling](#)

Do not render objects hidden by other ones.

#### [WIC] [High Dynamic Range](#)

Allows a rendered image to show more details when the scene contains both shadows and highlights.

#### [WIC] [Gamma correction](#)

Fix the gamma of the screen.

#### [WIC] [Normal mapping](#)

Show micro-geometry with lighting based on a texture.

#### [WIC] [Parallax mapping](#)

Adds micro-geometry to a model based on a texture.

### Physically based rendering (PBR)

#### [WIC] [Directional light](#)

Base exemple of PBR.

#### [WIC] [Multiple light types](#)

Test multiple light types with PBR.

#### [WIC] [Materials](#)

Uses PBR materials.

#### [WIC] [Image based lighting (IBL)](#)

Local illumination technique based on the scene.

#### [WIC] [Screen-space subsurface scattering (SSS)](#)

Shows hows lights can traverse some materials.

#### [WIC] [Hair](#)

Render hair and fur.

### Shadows

#### [WIC] [Directional shadow mapping](#)

Shadows created by a directional light.

#### [WIC] [Point shadow mapping](#)

Shadows created by point lights.

#### [WIC] [Screen space ambient occlusion (SSAO)](#)

Adds more shadows to micro-geometry based on vertex density.

#### [WIC] [Light map](#)

Baked global illumination based on raytracing.

### Animations

#### [WIC] [Linear transform](#)

Basic translation and rotation loaded from gltf.

#### [WIC] [Squeletal animation](#)

A basic character movement loaded from gltf.

### Post-Processing

#### [WIC] [Bloom](#)

Show a light with bloom effect.

#### [WIC] [Depth of field](#)

Blur the out of focus area of the image.

#### [WIC] [Motion blur](#)

Blur based on movement of the camera.

### Advanced

#### [WIC] [Particle system](#)

Shows emissive particles.

#### [WIC] [Instanced rendering](#)

Render a forest.

#### [WIC] [Tesselation](#)

Shows how an object contains more or less geometry based on camera distance.

#### [WIC] [Deferred shading](#)

Improve lighting performance with geometry buffer.

#### [WIC] [Deferred plus shading](#)

Improve lighting performance in a forward shading pipeline by dividing the screen on multiple areas.

## Based on

### Websites

- [Laurent Noel tutorials](http://igm.univ-mlv.fr/~lnoel/index.php?section=teaching&teaching=opengl&teaching_section=tds)

### Repos

- [Hazel engine](https://github.com/TheCherno/Hazel)

