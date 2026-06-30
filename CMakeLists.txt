cmake_minimum_required(VERSION 3.14)

project(laba_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


function(enable_strict_warnings target_name)
    if (MSVC)
        target_compile_options(${target_name} PRIVATE
                /W4
                /WX
                /utf-8
        )
    else()
        target_compile_options(${target_name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Werror
                -finput-charset=UTF-8
                -fexec-charset=UTF-8
        )
    endif()
endfunction()


add_library(laba4_core STATIC
        src/cardinal.cpp
        src/online_statistics.cpp
)

target_include_directories(laba4_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_strict_warnings(laba4_core)


add_executable(laba_4
        main.cpp
        src/console_interface.cpp
)

target_link_libraries(laba_4 PRIVATE
        laba4_core
)

enable_strict_warnings(laba_4)


include(FetchContent)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(laba_4_tests
        tests/cardinal_tests.cpp
        tests/generator_tests.cpp
        tests/online_statistics_tests.cpp
        tests/lazy_sequence_basic_tests.cpp
        tests/lazy_sequence_operations_tests.cpp
        tests/stream_tests.cpp
        tests/large_data_tests.cpp
)

target_link_libraries(laba_4_tests PRIVATE
        laba4_core
        GTest::gtest_main
)

enable_strict_warnings(laba_4_tests)

include(GoogleTest)

gtest_discover_tests(laba_4_tests)