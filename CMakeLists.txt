cmake_minimum_required(VERSION 3.15)
project(fft_project)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(fft ${SOURCES})

target_include_directories(fft PRIVATE include)
