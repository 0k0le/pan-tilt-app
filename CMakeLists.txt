cmake_minimum_required(VERSION 3.10)

project(camapp VERSION 1.0)

if(CMAKE_SYSTEM MATCHES Windows)
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_SYSTEM MATCHES Linux)
    add_compile_options(-Wall -Wextra -pedantic -O2)
endif()

# If windows set cmake config directories
if(CMAKE_SYSTEM MATCHES Windows)
    set(Qt5_DIR "C:\\Qt\\Qt5.12.12\\5.12.12\\msvc2017_64\\lib\\cmake\\Qt5")
    set(OpenCV_DIR "C:\\opencv\\build\\x64\\vc15\\lib")
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

# Pylon link directories
if(CMAKE_SYSTEM MATCHES Linux)
    link_directories(/opt/pylon/lib)
else()
    link_directories("C:\\Program Files\\Basler\\pylon 7\\Development\\lib\\x64")
endif()

# Handle include directories
if(CMAKE_SYSTEM MATCHES Linux)
    include_directories(${OpenCV_INCLUDE_DIRS} ./ src/include/ src/include/moc /opt/pylon/include)
else()
    include_directories("C:\\Program Files\\Basler\\pylon 7\\Development\\include" ${OpenCV_INCLUDE_DIRS} ./ src/include/ src/include/moc /opt/pylon/include)
endif()

# Create custom build for each moc dependency
set(MOC_SOURCE gui layout)
foreach(SOURCE IN LISTS MOC_SOURCE)
    message(STATUS "Setting up MOC ${SOURCE}")
    add_custom_command(
        OUTPUT "${PROJECT_SOURCE_DIR}/src/moc/${SOURCE}.moc"
        COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/src/moc && moc ${PROJECT_SOURCE_DIR}/src/include/${SOURCE}.hpp > ${PROJECT_SOURCE_DIR}/src/moc/${SOURCE}.moc
        DEPENDS "${PROJECT_SOURCE_DIR}/src/${SOURCE}.cpp" "${PROJECT_SOURCE_DIR}/src/include/${SOURCE}.hpp"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
endforeach()

add_executable(camapp src/main.cpp src/gui.cpp src/layout.cpp src/network.cpp src/pylon-video.cpp src/moc/gui.moc src/moc/layout.moc)

if(CMAKE_SYSTEM MATCHES Linux)
    target_link_libraries(camapp Qt5::Widgets ${OpenCV_LIBS} ${Pylon_LIBS} pthread pylonbase pylonutility GenApi_gcc_v3_1_Basler_pylon GCBase_gcc_v3_1_Basler_pylon)
else()
    target_link_libraries(camapp Qt5::Widgets ${OpenCV_LIBS} ${Pylon_LIBS} PylonBase_v6_3 PylonUtility_v6_3 GenApi_MD_VC141_v3_1_Basler_pylon GCBase_MD_VC141_v3_1_Basler_pylon wsock32 ws2_32)
endif()

message(STATUS ${PROJECT_SOURCE_DIR})