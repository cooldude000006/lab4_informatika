cmake_minimum_required(VERSION 3.15)

project(laba_4 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(laba_4
        main.cpp
        include/cardinal.h
        src/cardinal.cpp
        include/igenerator.h
        include/arithmetic_progression_generator.h
        include/lazy_sequence.h
        include/finite_sequence_generator.h
)

target_include_directories(laba_4 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
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