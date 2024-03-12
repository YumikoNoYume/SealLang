project(SealLang)
set(CMAKE_CXX_STANDARD 17)

file(GLOB src_files lib/src/*.cpp)
add_library(seallang_lib STATIC ${src_files})
add_executable(SealLangExe src/main.cpp)
target_link_libraries(SealLangExe PUBLIC seallang_lib)
include_directories(lib/inc)