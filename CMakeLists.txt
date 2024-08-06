cmake_minimum_required(VERSION 3.20)
project(Gl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: 
# set(CMAKE_CXX_COMPILER g++)

# Add subdirectories
add_subdirectory(src/main_files)
add_subdirectory(src/Shaders)
add_subdirectory(src/Textures)
