cmake_minimum_required(VERSION 3.14)
project(SealLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare( googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB src_files lib/src/*.cpp)
add_library(seallang_lib STATIC ${src_files})
add_executable(SealLangExe src/main.cpp)
target_link_libraries(SealLangExe seallang_lib)
include_directories(lib/inc)

enable_testing()
file(GLOB test_files test/*.cpp)
add_executable(SealLangTest ${test_files})
target_link_libraries(SealLangTest GTest::gtest_main seallang_lib)
include(GoogleTest)
gtest_discover_tests(SealLangTest)