# Roadmap

## Phase 1: STM32 Bring-Up
- verify flash/debug workflow in CLion
- confirm LED or GPIO control
- enable UART debug output
- run local UART echo test

## Phase 2: Arduino Sensor Node Bring-Up
- confirm Arduino serial output
- validate ultrasonic sensor read loop
- print raw distance values for sanity check

## Phase 3: STM32-Arduino UART Link
- verify TX/RX wiring and common ground
- confirm stable communication at `115200`
- exchange simple text commands before binary packets

## Phase 4: Binary Packet Protocol
- finalize command enum and payload rules
- implement frame encoder
- implement parser state machine
- validate malformed, truncated, and misaligned frames

## Phase 5: Sensor Request/Reply Flow
- STM32 sends sensor request packet
- Arduino performs ultrasonic measurement
- Arduino replies with `distance_mm`
- STM32 parses and logs the result

## Phase 6: Power Sequencing
- STM32 controls Arduino power through GPIO
- add startup delay around `300 ms`
- request status or sensor data after power-on
- detect and report Arduino no-response cases

## Phase 7: Portfolio Polish
- clean repository layout
- add diagrams and labeled photos
- document build and wiring steps
- capture serial logs and demo assets
- summarize lessons learned and future improvements
