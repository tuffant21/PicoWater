# Pico Water

This project automates the watering of your garden using a Raspberry Pi Pico. It leverages the Pico's Real-Time Clock (RTC) to set alarms for watering schedules and controls a solenoid valve to manage water flow.

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
   ```

4. Build the project:
   ```bash
   make
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
4. Define your watering schedule in `definitions.h`:
   ```c
   datetime_t ALARMS[] = {
       { .year = DATETIME_IGNORE, .month = DATETIME_IGNORE, .day = DATETIME_IGNORE, .dotw = SUNDAY, .hour = 6, .min = 0, .sec = 0 },
       { .year = DATETIME_IGNORE, .month = DATETIME_IGNORE, .day = DATETIME_IGNORE, .dotw = TUESDAY, .hour = 6, .min = 0, .sec = 0 },
       { .year = DATETIME_IGNORE, .month = DATETIME_IGNORE, .day = DATETIME_IGNORE, .dotw = FRIDAY, .hour = 6, .min = 0, .sec = 0 }
   };
   ```
5. Define how long you want the watering to run for (default is 300000):
    ```c
    #define SOLENOID_RUNTIME_MS 300000
    ```

## Project Structure
- **CMakeLists.txt**: CMake configuration file.
- **src/**: Source code directory.
  - **main.c**: Main application code.
  - **definitions.h**: Configuration and definitions.
- **pico_sdk_import.cmake**: Pico SDK import script.

## License
This project is licensed under the MIT License. See the [LICENSE](https://opensource.org/license/mit) file for details.