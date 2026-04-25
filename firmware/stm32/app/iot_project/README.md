# STM32-Arduino UART Sensor Gateway

An embedded systems portfolio project demonstrating MCU-to-MCU communication, UART protocol design, GPIO-based power control, ultrasonic sensor integration, and structured firmware development using an STM32F407G-DISC1 and an Arduino Nano.

## Why This Project Exists
This project was built to practice and demonstrate embedded fundamentals in a small but complete system:

- bringing up real hardware
- controlling external power with GPIO
- designing a simple binary UART protocol
- parsing and validating framed packets
- integrating a sensor through a secondary microcontroller
- debugging communication and timing issues methodically

The goal is not a large IoT platform. The goal is a clear, reproducible embedded project that shows practical engineering judgment.

## What This Project Demonstrates
- STM32 firmware development with HAL, FreeRTOS-style tasks, and CMake/CLion workflow
- Arduino Nano firmware development for a sensor node role
- UART communication at `115200` baud
- custom packet framing with start byte, length, command, payload, and end byte
- byte queue buffering and parser state machine design
- GPIO-controlled power sequencing for a secondary MCU
- sensor request/reply flow between two microcontrollers
- bring-up, validation, and debugging discipline suitable for interviews and portfolio review

## System Overview
The system is split into two boards with clear responsibilities:

- `STM32F407G-DISC1`
  - main controller
  - powers the Arduino through a GPIO-controlled switch stage
  - sends UART commands
  - receives, parses, and processes sensor data
- `Arduino Nano`
  - sensor node
  - interfaces directly with the ultrasonic sensor
  - replies to STM32 requests using a simple framed binary protocol
- `Ultrasonic sensor`
  - connected to Arduino digital pins
  - returns distance data used as the application payload

## Hardware Used
- `STM32F407G-DISC1`
- `Arduino Nano`
- ultrasonic/sonar sensor module
- `9V` battery supply
- `9V` to `5V` regulator
- GPIO-controlled P-MOSFET power switch for Arduino power control
- UART wiring between STM32 and Arduino with common ground

## Architecture Summary
### Hardware Architecture
1. The battery/regulator stage powers the system.
2. The STM32 controls whether the Arduino receives power.
3. The Arduino reads the ultrasonic sensor.
4. The STM32 and Arduino exchange framed UART packets.

### Firmware Architecture
STM32 side:

- `main.c`
  - board and peripheral initialization
- `drv_uart`
  - non-blocking UART transmit and interrupt-driven receive
- `byte_queue`
  - byte-oriented receive buffering
- `uart_packet`
  - packet constants, validation, encoder, and parser state machine
- `iot_proc`
  - application task that consumes received frames and dispatches commands

Arduino side:

- serial receive loop
- matching packet parser
- ultrasonic sensor driver
- sensor reply packet builder

## UART Protocol
First-version frame format:

- `Start`: `0xAA`
- `Length`: `2 bytes`, little-endian payload length
- `CMD`: `1 byte`
- `Data`: `n bytes`
- `End`: `0xBB`

Command set:

- `0x01` sensor data request
- `0x02` reboot request
- `0x81` sensor data reply
- `0x82` reboot reply

First-version payload rules:

- request packets use zero-length payloads
- sensor data reply uses `2 bytes` little-endian `distance_mm`
- reboot reply uses `1 byte` status code

Example frames:

- sensor request: `AA 00 00 01 BB`
- sensor reply for `1234 mm`: `AA 02 00 81 D2 04 BB`

CRC is intentionally omitted in the first version to keep bring-up simple. It can be added later once the link is stable.

## Current Status
This repository currently contains the STM32 application code, protocol utilities, and task structure in progress.

Current codebase highlights:

- CubeMX-generated STM32 project configured for CLion/CMake
- interrupt-driven UART receive path and byte queue buffering
- protocol framing and parser skeleton under `Core/Inc/util` and `Core/Src/util`
- FreeRTOS-style processing task in `iot_proc`

Scope note:

- project notes referenced a boot/startup flow and `USART2`
- the current STM32 code in this repository uses `USART3`
- the portfolio version should preserve the startup/power-sequencing concept, while documenting the final hardware/UART mapping actually used

## Development Roadmap
1. STM32 bring-up: LED/GPIO test, UART debug output, UART echo test
2. Arduino bring-up: serial echo, ultrasonic read, distance print
3. STM32-Arduino link validation: wiring, common ground, baud verification
4. Binary protocol implementation: encode, parse, validate, recover from bad frames
5. Sensor request/reply flow: STM32 request, Arduino measurement, STM32 parse
6. Power sequencing: GPIO-controlled Arduino power, startup delay, timeout handling
7. Portfolio polish: diagrams, logs, testing notes, demo assets, cleaned repository structure

## Testing Plan
- UART loopback and echo tests on each MCU independently
- STM32-to-Arduino command/reply tests at `115200`
- parser tests with valid, truncated, misaligned, and malformed frames
- power-sequencing tests with startup delay and no-response handling
- sensor sanity checks against known distances
- logic-analyzer or serial-log captures for documentation

## Build And Run
Build steps are intentionally documented as a placeholder until the repository is cleaned into a single top-level portfolio layout.

Planned documentation:

- STM32 toolchain and CLion/CMake setup
- Arduino IDE or CLI build steps
- UART wiring table
- expected serial logs for bring-up
- demo procedure for powering the Arduino, requesting sensor data, and observing replies

## What I Learned
- how to structure a simple MCU-to-MCU protocol so it is easy to debug on real hardware
- why fixed-size payloads and explicit validation simplify early bring-up
- how interrupt-driven UART receive can be paired with queue-based application parsing
- how power sequencing affects secondary MCU startup and communication timing
- how to document hardware assumptions and test evidence clearly enough for portfolio review

## Future Improvements
- add protocol timeout handling and retry logic
- move from provisional parser skeleton to fully integrated command dispatcher
- add parser-focused host-side tests
- document final wiring with diagrams and labeled photos
- add optional CRC after basic communication is stable
- normalize the repository into a clean root layout containing STM32, Arduino, docs, diagrams, and test assets

## Recruiter And Interviewer Highlights
- real embedded hardware integration across two microcontrollers
- clean separation between transport, protocol, and application logic
- explicit packet validation and staged debugging plan
- practical focus on communication reliability and system startup behavior
- documentation intended to show both technical implementation and engineering process

## Assumptions
- The project notes described in the original materials were not available in this workspace during this documentation pass.
- This README is based on the current codebase plus the project details provided in the task description.
- Where the notes and code differ, the difference is documented rather than hidden.
