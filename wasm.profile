include(default)

[settings]
os=Emscripten
arch=wasm
compiler=clang
compiler.version=6.0
compiler.libcxx=libc++
compiler.cppstd=17

[build_requires]
emsdk_installer/1.38.29@bincrafters/stable
ninja_installer/1.9.0@bincrafters/stable
