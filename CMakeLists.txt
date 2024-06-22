cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pico_water_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

file(GLOB_RECURSE SOURCES_H src/*.h)
file(GLOB_RECURSE SOURCES_C src/*.c)

add_executable(pico_water
  src/main.c
  ${SOURCES_H}
  ${SOURCES_C}
)
pico_set_program_name(pico_water "PicoWater")
pico_set_program_version(pico_water "1.0.0")
pico_enable_stdio_uart(pico_water 0)
pico_enable_stdio_usb(pico_water 1)
pico_add_extra_outputs(pico_water)

# if cmake is run like this "cmake -DPICO_BOARD=pico_w ..", this if statement will evaluate to true
if(DEFINED PICO_BOARD AND PICO_BOARD STREQUAL "pico_w")
  message("PICO_BOARD is set to 'pico_w'")
  target_compile_definitions(pico_water PRIVATE PICO_W=1)
  target_link_libraries(pico_water
    pico_stdlib
    pico_multicore
    hardware_rtc
    pico_cyw43_arch_none
    )
else()
  message("PICO_BOARD is not set")
  target_compile_definitions(pico_water PRIVATE PICO_W=0)
  target_link_libraries(pico_water
    pico_stdlib
    pico_multicore
    hardware_rtc
    )
endif()