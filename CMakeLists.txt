cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)

include(cmake/pico_sdk_import.cmake)

project(RobotArm)

pico_sdk_init()

file(GLOB_RECURSE robot_arm_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE robot_arm_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/robotarm/*.h)

add_library(robotarm ${robot_arm_sources} ${robot_arm_headers})
target_include_directories(robotarm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(robotarm pico_stdlib hardware_spi)

add_executable(main main.cpp)
target_link_libraries(main robotarm)

pico_enable_stdio_usb(main 1) # enable serial output over USB
pico_enable_stdio_uart(main 0) # disable serial output over UART

# Create uf2 file
pico_add_extra_outputs(main)
