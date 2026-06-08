cmake_minimum_required(VERSION 3.31.6) # set minimum CMake version

# project name
project(ProjectGNA)

# check C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler-specific extensions for "clean" code

# program will not compile if had at least one warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# finding all .cpp files in src directories and add their paths to one MY_FILES(name for sources) list
file(GLOB MY_FILES "src/*.cpp") # GLOB - mode of searching files by patterns

# create executable project file
add_executable(ProjectGNAexecutable ${MY_FILES})

# for include where to find files
# PRIVATE - this means the include directories are only used for compiling ProjectGNAexecutable
target_include_directories(ProjectGNAexecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ProjectGNAexecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# to link compiled libparameters.so library to ProjectGNAexecutable
target_link_libraries(ProjectGNAexecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libparameters.so)