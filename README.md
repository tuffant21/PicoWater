# Pico Water

This project automates the watering of your garden using a Raspberry Pi Pico. It leverages the Pico's Real-Time Clock (RTC) to set alarms for watering schedules and controls a solenoid valve to manage water flow.

You can check out a video walkthrough of the code for version 1.0.0 on my YouTube channel [here](https://youtu.be/9xKd_dKpwEM?si=SkYf2hw4Hm9uImdd).

<iframe width="560" height="315" src="https://www.youtube.com/embed/9xKd_dKpwEM?si=0ycfxvVp_n1SlPHy" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## Table of Contents
- [Features](#features)
- [Requirements](#requirements)
- [Setup](#setup)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [License](#license)

## Features
- Automated garden watering using a solenoid valve.
- Configurable watering schedules via RTC alarms.
- Supports both Raspberry Pi Pico and Pico W (with Wi-Fi functionality).
- Supports USB communication to control pico

## Requirements
- Raspberry Pi Pico or Pico W
- Solenoid valve
- Necessary electronic components (wires, resistors, etc.)
- CMake 3.13 or later
- Raspberry Pi Pico SDK 1.4.0 or later

## Setup
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/pi_water_pico_project.git
   cd pi_water_pico_project
   ```

2. Initialize the Pico SDK submodule:
   ```bash
   git submodule update --init
   ```

3. Configure your project:
   ```bash
   mkdir build
   cd build
   cmake ..

   # or if you're using pico_w
   cmake -DPICO_BOARD=pico_w ..
   ```

4. Build the project:
   ```bash
   make -j4
   ```

## Usage
1. Flash the compiled program to your Raspberry Pi Pico or Pico W.
2. Connect the solenoid valve to the specified GPIO pin (default is pin 16).
3. Set the RTC to the current date and time in `definitions.h`:
   ```c
   #define TODAYS_YEAR 2024
   #define TODAYS_MONTH 5
   #define TODAYS_DAY 25
   #define TODAYS_DOTW SATURDAY
   #define TODAYS_HOUR 1
   #define TODAYS_MIN 30
   #define TODAYS_SEC 0
   ```
4. Define your watering schedule in `initial_alarms.h`:
   ```c
   datetime_t INITIAL_ALARMS[] = {
      { 
         .year = DATETIME_IGNORE, 
         .month = DATETIME_IGNORE, 
         .day = DATETIME_IGNORE, 
         .dotw = SUNDAY, 
         .hour = 6, 
         .min = 0, 
         .sec = 0 
      },
      { 
         .year = DATETIME_IGNORE,
         .month = DATETIME_IGNORE, 
         .day = DATETIME_IGNORE, 
         .dotw = TUESDAY, 
         .hour = 6, 
         .min = 0, 
         .sec = 0 
      },
      { 
         .year = DATETIME_IGNORE, 
         .month = DATETIME_IGNORE, 
         .day = DATETIME_IGNORE, 
         .dotw = FRIDAY, 
         .hour = 6, 
         .min = 0, 
         .sec = 0 
      }
   };
   ```
5. Define how long you want the watering to run for (default is 300000):
    ```c
    #define SOLENOID_RUNTIME_MS 300000
    ```
6. Control the Pico via USB
  * connect a usb to the pico
  * run the following command in a terminal  `minicom -b 115200 -D /dev/ttyACM0 -c on`
  * type `help` to get started

## Project Structure
- **build/**: Compiled source directory.
- **src/**: Source code directory.
  - **alarms.c**: Holds and defines all of the application alarms
  - **alarms.h**: Headers for alarms.c
  - **core_entries.c**: Holds any interrupts or core 1 entries
  - **core_entries.h**: Headers for core_entries.c
  - **definitions.h**: Configuration and definitions.
  - **initial_alarms.h**: Holds all of the initial alarms a user wants the program to start with
  - **logger.c**: Handles printing color and arguments to console
  - **logger.h**: Headers for logger.c
  - **main.c**: Main application code.
  - **on_board_led.c**: Handles setting the GPIO value for the on board led
  - **on_board_led.h**: Headers for on_board_led.c
  - **usb_comms.c**: USB Communication between a host and the pico
  - **usb_comms.h** Headers for usb_comms.c
- **CHANGELOG.md**: History of project versions and changes
- **CMakeLists.txt**: CMake configuration file.
- **pico_sdk_import.cmake**: Pico SDK import script.
- **README.md**: Project setup and information

## Special Thanks
Thank you to the "Learn Embedded Systems" YouTube channel for the tutorial on how to make a Simple Web Server in C. 
Make sure to check out their YouTube video that I followed [here](https://youtu.be/C_O0MXGBHsw?si=Nnk5kwAcImsK2u_w)

<iframe width="560" height="315" src="https://www.youtube.com/embed/C_O0MXGBHsw?si=Nnk5kwAcImsK2u_w" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## License
This project is licensed under the MIT License. See the [LICENSE](https://opensource.org/license/mit) file for details.