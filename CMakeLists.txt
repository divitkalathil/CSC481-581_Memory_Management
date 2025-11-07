# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(CustomAllocatorDemo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find SDL3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)

# Create the executable from source files
add_executable(
    CustomAllocatorDemo
    src/main.cpp
    src/CustomAllocator.cpp
    src/Bullet.cpp
    src/BulletPool.cpp
)

# Link SDL3 library
target_link_libraries(CustomAllocatorDemo PRIVATE PkgConfig::SDL3)

# Add src directory to include path
target_include_directories(CustomAllocatorDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
