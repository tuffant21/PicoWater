cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pi_water_pico_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

file(GLOB_RECURSE SOURCES_H src/*.h)
file(GLOB_RECURSE SOURCES_C src/*.c)

add_executable(pi_water_pico
  src/main.c
  ${SOURCES_HPP}
  ${SOURCES_CPP}
)
pico_set_program_name(pi_water_pico "PiWater")
pico_set_program_version(pi_water_pico "1.0")
pico_enable_stdio_uart(pi_water_pico 0)
pico_enable_stdio_usb(pi_water_pico 1)
pico_add_extra_outputs(pi_water_pico)

# change this variable to FALSE if you are not using a PICO_W
set(PICO_W TRUE)

if(PICO_W)
  set(PICO_BOARD pico_w CACHE STRING "Board type")

  target_link_libraries(pi_water_pico pico_stdlib pico_multicore hardware_rtc pico_cyw43_arch_none)
else()
  target_link_libraries(pi_water_pico pico_stdlib pico_multicore hardware_rtc)
endif()