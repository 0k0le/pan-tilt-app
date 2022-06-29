cmake_minimum_required(VERSION 3.10)

project(camapp VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -O2)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

link_directories(/opt/pylon/lib)
include_directories(${Pylon_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ./ src/include/ src/include/moc /opt/pylon/include)

add_executable(camapp src/main.cpp src/gui.cpp src/layout.cpp src/network.cpp src/pylon-video.cpp)

target_link_libraries(camapp Qt5::Widgets ${OpenCV_LIBS} ${Pylon_LIBS} pthread pylonbase pylonutility GenApi_gcc_v3_1_Basler_pylon GCBase_gcc_v3_1_Basler_pylon)