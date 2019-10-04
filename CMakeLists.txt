# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// SETTINGS //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

# Set standards
cmake_minimum_required(VERSION 3.10)
project(opengl-playground C CXX)
set(CMAKE_CXX_STANDARD 17)
set(default_build_type "Release")

# Setup Conan
if (EMSCRIPTEN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1 -s BINARYEN_TRAP_MODE='clamp' -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2 -s DEMANGLE_SUPPORT=1 -std=c++17 -g4 --profiling --preload-file ../res")
else()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake")
    file(DOWNLOAD
        "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()
    include(${CMAKE_BINARY_DIR}/conan.cmake)
    conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP BUILD missing)
endif()

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

file(GLOB_RECURSE MY_SOURCES src/*)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////// DEPENDENCIES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

find_package(OpenGL REQUIRED)

# ------------------------- BUILD DEPENDENCIES IF NEEDED ----------------------

add_library(GLAD STATIC "lib/glad/src/glad.c")
add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.cpp")
add_library(IMGUI STATIC "lib/imgui/imgui_impl_opengl3.cpp" "lib/imgui/imgui_impl_sdl.cpp")

# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// INCLUDES //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

include_directories(
    ${OPENGL_INCLUDE_DIR}
    lib/glad/include
    lib
    src
    res
)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// BUILD ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(${PROJECT_NAME} ${MY_SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    ${CONAN_LIBS}
    ${OPENGL_LIBRARIES}
    GLAD
    STB_IMAGE
    IMGUI
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()
