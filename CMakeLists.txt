cmake_minimum_required(VERSION 3.15) # https://gitlab.kitware.com/cmake/cmake/-/merge_requests/3250
project(KryogenicV2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCES
        "Source/*.hpp"
        "Source/*.cpp"
)

add_executable(KryogenicV2 ${SOURCES})

set_target_properties(KryogenicV2 PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories(KryogenicV2 PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        "include"
        "Source"
)

target_link_libraries(KryogenicV2 PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        spdlog::spdlog
)

target_precompile_headers(KryogenicV2 PRIVATE
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(KryogenicV2 PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror

            -Wno-pre-c++17-compat
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(KryogenicV2 PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror

            -Wno-pre-c++17-compat
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(KryogenicV2 PRIVATE
            /W4
            /WX
    )
endif ()
