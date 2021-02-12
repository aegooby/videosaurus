
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

### VCpkg ###
set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

project(videosaurus)

set(CMAKE_EXPORT_COMPILE_COMMANDS BOOL TRUE)

### C++20 ###
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Flags ###
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVS_DEBUG -Og")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra")

### Source files ###
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/source/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/source/*.cpp)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

include_directories(${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

### ZeroMQ ###
find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cppzmq cppzmq-static)

### OpenCV ###
# find_package(OpenCV CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)

### N-API ###
execute_process(COMMAND node -p "require('node-addon-api').include" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
add_definitions(-DNAPI_VERSION=6)