# Changelog

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