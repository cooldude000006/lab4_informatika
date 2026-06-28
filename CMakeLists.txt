cmake_minimum_required(VERSION 3.15)

project(laba_4 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(laba_4
        main.cpp
)

if (MSVC)
    target_compile_options(laba_4 PRIVATE
            /W4
            /WX
    )
else()
    target_compile_options(laba_4 PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endif()