# Changelog

## [1.2.1] - 2024-07-06

### Added
* version-check.yml to ensure version is updated correctly for each pr

### Changed
* Updated README.md to reflect file structure changes

### Fixed
* N/A

### Removed
* N/A

## [1.2.0] - 2024-07-06

### Added
* `WIFI_SSID` that can be added as a build argument to CMake
* `WIFI_PASSWORD` that can be added as a build argument to CMake
* `makefsdata.py` to convert html files into raw bytes
* Screenshots of the application to the screenshots folder
* `index.shtml` to the html_files folder for pico_w webserver
* `lwipopts.h` for light weight ip options
* `src/cgi.c` file to handle updates to the webserver
* `src/helpers.c` file to help with various helper functions
* `src/ssi.c` file to return data for the application
* `src/sys_info.c` file to retrieve system information shared between the ssi and usb_comms

### Changed
* gitignore to ignore new htmldata.c
* Pico SDK minimum required version to 1.5.0
* Updated CMakeLists.txt
* Updated README.md to reflect new changes to the system
* Moved water_plants rtc interrupt to `src/alarms.c`
* Updated alarm functionality to accomodate multiple files updating alarms
* Updated usb comms to have the additional functionality
  * get-uptime
  * get-temp
  * get-ip
  * connect-to-network

### Fixed
* Include guards to determine the correct value of PICO_W

### Removed
* `src/core_entries.c` to let `alarms.c` handle interrupts directly
* `src/on_board_led.c` and moved to helper functions file

## [1.1.1] - 2024-21-24

### Added
* N/A

### Changed
* N/A

### Fixed
* Fixed missing import for pico_w in main

### Removed
* N/A

## [1.1.0] - 2024-21-24

### Added
* Added USB Communication
* Added logger to log debug, info, warn, and error
* Added dedicated on_board_led file
* Added dedicated initial_alarms file
* Added dedicated core_entries file
* Added an alarms file to manage program alarms

### Changed
* Update README.md

### Fixed
* Fix an incorrect SOURCES_HPP and SOURCES_CPP to SOURCES_H and SOURCES_C

### Removed
* N/A

## [1.0.1] - 2024-06-16

### Added
* Preprocessor definition in `CMakeLists.txt` for PICO_W variable

### Changed
* CMake requires the board type to be defined via `-DPICO_BOARD=` for pico_w to build now
* Updated README to reflect new build process

### Fixed
* Known issue in v1.0.0 that was in the Known Issues section of the `README.md`

### Removed
* PICO_W variable in `CMakeLists.txt`
* PICO_W variable in `definitions.txt`
* Known Issues section from `README.md`. New build process and preprocessor definition should now fix this

## [1.0.0] - 2024-06-15

### Added
* Initial release of the Pi Water Pico Project.
* Automated garden watering system using Raspberry Pi Pico.
* Real-Time Clock (RTC) based scheduling for watering.
* Support for both Raspberry Pi Pico and Pico W (with Wi-Fi functionality).
* Onboard LED control for indicating watering status.
* Solenoid valve control for water management.
* Configurable watering schedules via definitions.h.
* Multicore support to handle watering tasks.

### Changed
* N/A

### Fixed
* N/A

### Removed
* N/A