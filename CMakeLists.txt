cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pico_water_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

if(DEFINED PICO_BOARD AND PICO_BOARD STREQUAL "pico_w")
  message("PICO_BOARD is set to 'pico_w'")

  # Check for wifi credentials
  set(WIFI_SSID "${WIFI_SSID}" CACHE INTERNAL "WiFi SSID for connecting to network")
  set(WIFI_PASSWORD "${WIFI_PASSWORD}" CACHE INTERNAL "WiFi password for connecting to network")

  if ("${WIFI_SSID}" STREQUAL "")
    message(WARNING "WIFI_SSID is not defined")
  endif()

  if ("${WIFI_PASSWORD}" STREQUAL "")
    message(WARNING "WIFI_PASSWORD is not defined")
  endif()

  # Execute the python script to build html files
  find_program(PYTHON_EXECUTABLE NAMES python3 python)

  if(NOT PYTHON_EXECUTABLE)
    message(FATAL_ERROR "Python is not installed or not found in PATH")
  endif()

  # Check if the Python script exists before attempting to run it
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/makefsdata.py")
    message("Running makefsdata python script")
    execute_process(COMMAND
      ${PYTHON_EXECUTABLE} makefsdata.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  else()
    message(FATAL_ERROR "makefsdata.py script not found")
  endif()
else()
  message("PICO_BOARD is not pico_w, assuming standard pico")
  message("NOT running makefsdata python script")
endif()

set(PROGRAM_NAME pico_water)

file(GLOB_RECURSE SOURCES src/*.[ch])
add_executable(${PROGRAM_NAME}
  src/main.c
  ${SOURCES}
)

target_include_directories(${PROGRAM_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_set_program_name(${PROGRAM_NAME} "PicoWater")
pico_set_program_version(${PROGRAM_NAME} "1.0.0")
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)
pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_add_extra_outputs(${PROGRAM_NAME})

# if cmake is run like this "cmake -DPICO_BOARD=pico_w ..", this if statement will evaluate to true
if(DEFINED PICO_BOARD AND PICO_BOARD STREQUAL "pico_w")
  target_compile_definitions(${PROGRAM_NAME} PRIVATE PICO_W=1)
  target_compile_definitions(${PROGRAM_NAME} PRIVATE WIFI_SSID=${WIFI_SSID})
  target_compile_definitions(${PROGRAM_NAME} PRIVATE WIFI_PASSWORD=${WIFI_PASSWORD})
  set(PICO_LIBS
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
  )
else()
  target_compile_definitions(${PROGRAM_NAME} PRIVATE PICO_W=0)
endif()

target_link_libraries(${PROGRAM_NAME}
  ${PICO_LIBS}
  pico_stdlib
  pico_multicore
  hardware_rtc
  hardware_adc
)
