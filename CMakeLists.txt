cmake_minimum_required(VERSION 3.15)
project(fft_project)

set(CMAKE_CXX_STANDARD 20)

# Собираем все .cpp файлы
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(fft ${SOURCES})

target_include_directories(fft PRIVATE include)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(portaudio CONFIG REQUIRED)


target_link_libraries(fft PRIVATE 
    glfw
    OpenGL::GL
    portaudio
)