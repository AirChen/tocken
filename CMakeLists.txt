cmake_minimum_required(VERSION 3.5)

SET(PROJ token)
project(${PROJ} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(COMPILER_FLAGS_RELEASE)

file(GLOB_RECURSE SRC "src/*.c" "src/*.cpp" "src/*.hpp" "src/*.h")
# add_library(${PROJ} STATIC ${SRC})
add_executable(${PROJ} ${SRC})

########################################################################################
# benchmark
# set(BC_PROJ "token-benchmark")
# add_subdirectory("thirdparty/benchmark")

# file(GLOB_RECURSE BC_SRC "benchmark/*.cpp")

# add_executable(${BC_PROJ} ${BC_SRC})
# target_link_libraries(${BC_PROJ} ${PROJ} benchmark::benchmark)
