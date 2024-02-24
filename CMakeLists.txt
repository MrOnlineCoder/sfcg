cmake_minimum_required(VERSION 3.16)
project(sfcg LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_SFCG_EXAMPLE "Build example" ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(SOURCES
    src/Init.cpp
    src/Shape.cpp
    src/RenderTarget.cpp
    src/RectangleShape.cpp
    src/GeometryCache.cpp
    src/GLCheck.cpp
    src/Shader.cpp
    src/VertexBuffer.cpp
    src/CircleShape.cpp
    src/Sprite.cpp
    src/Text.cpp
    src/RenderWindow.cpp
    src/ConvexShape.cpp
)

add_library(sfcg STATIC ${SOURCES})

target_link_libraries(sfcg PRIVATE sfml-system sfml-window sfml-graphics)
target_compile_features(sfcg PRIVATE cxx_std_17)

target_include_directories(sfcg PRIVATE ./include)

target_compile_definitions(sfcg PRIVATE GL_SILENCE_DEPRECATION)

if(BUILD_SFCG_EXAMPLE)
    add_subdirectory(example)
endif()

install(TARGETS sfcg)