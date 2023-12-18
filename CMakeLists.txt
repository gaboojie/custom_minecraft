cmake_minimum_required(VERSION 3.6)
project(minecraft)

#if (APPLE)
#    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit")
#    # -framework OpenGL -framework CoreVideo
#    add_compile_definitions(GL_SILENCE_DEPRECATION)
#endif ()

#add_definitions(dede
#        -DTW_STATIC
#        -DTW_NO_LIB_PRAGMA
#        -DTW_NO_DIRECT3D
#        -DGLEW_STATIC
#        -D_CRT_SECURE_NO_WARNINGS
#)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES ${PROJECT_SOURCE_DIR} ./src/main/app.c src/shader/vao.c src/io/file-loader.c ./src/shader/shader.c src/shader/texture.c src/math/transform.c headers/shader/model.h src/shader/model.c src/math/math-util.c src/shader/camera.c src/io/input.c src/structure/interval-tree.c src/world/world-renderer.c src/world/world.c src/world/chunk.c src/world/world-generator.c headers/world/chunk.h headers/world/world-generator.h headers/world/world-renderer.h src/world/block.c headers/world/block.h headers/world/world.h src/text/text-renderer.c headers/text/text-renderer.h src/text/font-generator.c headers/text/font-generator.h src/text/font.c headers/text/font.h headers/structure/linked-list.h src/structure/linked-list.c src/structure/linked-list.c headers/text/text.h src/text/text.c src/structure/hash-map.c headers/structure/hash-map.h)
add_executable(minecraft ${SOURCE_FILES})
target_include_directories(minecraft PUBLIC ./headers ./headers/main ./headers/shader ./headers/io ./headers/math ./headers/structure ./headers/world ./headers/text)

# GLAD
add_subdirectory(./libs/glad/)
target_link_libraries(minecraft glad)

# STB
include_directories(./libs/stb-master)
link_directories(./libs/stb-master)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(./libs/glfw-3.3.8)
target_link_libraries(minecraft glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(minecraft OpenGL::GL)

# GLM
add_subdirectory(libs/cglm-master/ EXCLUDE_FROM_ALL)
target_link_libraries(minecraft cglm_headers)