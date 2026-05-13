cmake_minimum_required(VERSION 3.15)
project(quoridor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Portable Build: Use only local include/ and lib/ directories
# No external dependencies required - everything is included in the project

# Include directories (all project-local)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ai)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/controller)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/persistence)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ui)

# Link directories (only project-local libraries)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Collect all source files from src directory and subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameConfig.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ai/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/controller/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/persistence/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp"
)

# Create the executable
add_executable(quoridor ${SOURCES})

# Link raylib and system libraries
target_link_libraries(quoridor raylib opengl32 gdi32 winmm)

# static link c++ and gcc runtimes for portable executable
target_link_options(quoridor PRIVATE -static-libstdc++ -static-libgcc)

# Compiler flags
if(MSVC)
    target_compile_options(quoridor PRIVATE /W4)
else()
    target_compile_options(quoridor PRIVATE -Wall)
endif()

# For debug builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(quoridor PRIVATE -g -O0)
else()
    target_compile_options(quoridor PRIVATE -O2)
endif()
