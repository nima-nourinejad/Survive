cmake_minimum_required(VERSION 3.22.1)
project(ServerApp LANGUAGES CXX)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files in the current directory with extensions .cpp and .h
file(GLOB SOURCES "*.cpp" "*.h")

# Define the executable target
add_executable(ServerApp ${SOURCES})

# Optionally enable shared library support (not required if you're just creating an executable)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Custom command to copy assets if necessary (optional, remove if you have no assets)
# add_custom_command(
#     TARGET ServerApp
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
#     COMMENT "Copying assets directory"
# )

