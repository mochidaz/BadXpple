cmake_minimum_required(VERSION 3.20)
project(badxpple)

add_executable(badxpple "src/main.cpp")

target_link_libraries(badxpple PRIVATE X11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPULSE REQUIRED IMPORTED_TARGET libpulse)
target_link_libraries(badxpple PRIVATE PkgConfig::LIBPULSE)

find_package(ZLIB REQUIRED)
target_link_libraries(badxpple PRIVATE ZLIB::ZLIB)
