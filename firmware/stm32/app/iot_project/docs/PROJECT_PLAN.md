# Project Plan

## Overview
`STM32-Arduino UART Sensor Gateway` is a small embedded sensing system in which an STM32 controls power to an Arduino Nano, communicates with it over UART, requests ultrasonic sensor data, parses structured replies, and coordinates the overall application flow.

This is sized deliberately as a portfolio project rather than a product. The target is a clean demonstration of embedded engineering fundamentals with real hardware, not feature breadth.

## What This Project Demonstrates
- firmware development on two microcontroller platforms
- GPIO control and power sequencing
- UART bring-up, debug logging, and packetized communication
- custom protocol design with bounded payloads and validation rules
- queue-based receive handling and parser state machines
- sensor-node architecture with MCU-to-MCU messaging
- disciplined debugging, staged bring-up, and test documentation

## Technical Architecture Summary
The STM32 is the system coordinator. It owns the startup sequence, controls the Arduino power switch, transmits commands, receives reply bytes, and processes completed packets.

The Arduino Nano is a dedicated sensor node. It reads the ultrasonic sensor, converts the measurement into a simple binary payload, and returns that payload to the STM32 in a framed reply packet.

The link layer is asynchronous UART at `115200` baud. The application layer is a small binary protocol with a start byte, payload length, command byte, payload bytes, and end byte.

## Hardware Architecture Summary
- power source: `9V` battery
- regulated rail: `5V` for external logic and Arduino power path
- STM32 board: `STM32F407G-DISC1`
- secondary MCU: `Arduino Nano`
- sensor: ultrasonic/sonar module connected to Arduino digital pins
- power control: STM32 GPIO drives a P-MOSFET-based switch for Arduino power enable
- communication: UART between STM32 and Arduino with shared ground

## Firmware Architecture Summary
STM32 modules:

- `main`
  - hardware init and top-level scheduler start
- `drv_uart`
  - UART abstraction, interrupt-driven RX, async TX
- `byte_queue`
  - RX buffering between interrupt and task context
- `uart_packet`
  - command enum, packet definition, encode function, parser skeleton, validation
- `iot_proc`
  - application task, queue consumption, command dispatch
- planned modules
  - `arduino_power`
  - `sensor_gateway`
  - `system_log`

Arduino modules:

- `sensor_node.ino`
  - setup and loop
- `protocol`
  - framing, parsing, command handling
- `ultrasonic_sensor`
  - trigger/echo measurement and distance conversion

## Communication Protocol Summary
Frame:

- `Start`: `0xAA`
- `Length`: payload length, `uint16_t`, little-endian
- `CMD`: `uint8_t`
- `Data`: `n bytes`
- `End`: `0xBB`

Commands:

- request `0x01`: sensor data request
- request `0x02`: reboot request
- reply `0x81`: sensor data reply
- reply `0x82`: reboot reply

First-version payload choices:

- request packets carry no payload
- sensor data reply carries `uint16_t distance_mm`
- reboot reply carries `uint8_t status`

## Development Milestone Plan
1. Board bring-up
   - verify STM32 build, flash, LED, and GPIO control
   - enable UART debug output
2. Arduino sensor-node bring-up
   - validate serial output
   - validate ultrasonic measurement locally
3. Point-to-point UART validation
   - verify TX/RX, common ground, and baud rate
   - confirm echo or simple text exchange
4. Binary protocol integration
   - enable framed request/reply packets
   - validate parser recovery from bad bytes
5. Application behavior
   - request sensor data
   - parse and log distance values
6. Power sequencing
   - power Arduino from STM32-controlled switch
   - wait startup delay
   - handle timeout or missing replies
7. Portfolio polish
   - diagrams, test logs, photos, and cleaned documentation

## Testing And Debugging Plan
- unit-level protocol checks
  - valid frame encode/decode
  - invalid start, invalid end, invalid length, invalid command
- integration-level UART checks
  - STM32 echo
  - Arduino echo
  - end-to-end request/reply
- hardware checks
  - confirm regulator output
  - confirm MOSFET power switching behavior
  - confirm UART voltage compatibility and ground reference
- evidence capture
  - serial logs
  - logic-analyzer screenshots if available
  - measured distance examples at known targets

## Documentation Plan
- `README.md`
  - project overview, architecture, status, and portfolio framing
- `docs/architecture.md`
  - hardware and firmware block view
- `docs/hardware.md`
  - wiring, power flow, signal assumptions
- `docs/protocol.md`
  - packet format and payload rules
- `docs/testing.md`
  - test matrix and observed results
- `docs/bringup-log.md`
  - chronological bring-up notes
- `docs/troubleshooting.md`
  - common failure modes and fixes
- `docs/portfolio-summary.md`
  - recruiter-friendly summary of what the project demonstrates

## Recommended GitHub Repository Structure
This is the recommended clean root layout for the portfolio version of the repository:

```text
stm32-arduino-uart-sensor-gateway/
  README.md
  LICENSE
  docs/
    PROJECT_PLAN.md
    architecture.md
    hardware.md
    protocol.md
    roadmap.md
    testing.md
    bringup-log.md
    troubleshooting.md
    portfolio-summary.md
    repository-layout.md
  diagrams/
    system-block-diagram.png
    uart-sequence-diagram.png
    packet-format.png
  images/
    breadboard/
    logic-analyzer/
  firmware/
    stm32/
      app/
      Core/
      Drivers/
      Middlewares/
      USB_HOST/
      CMakeLists.txt
    arduino/
      sensor_node/
        sensor_node.ino
        protocol.h
        protocol.cpp
        ultrasonic_sensor.h
        ultrasonic_sensor.cpp
  shared/
    protocol/
      packet-spec.md
  tests/
    protocol-parser-tests/
  tools/
    packet_decoder.py
```

## Assumptions
- The project notes PDF referenced in the task was not present in the writable workspace during this pass.
- The task description is therefore treated as the primary project specification for documentation.
- Notes mention `USART2`, while current STM32 code uses `USART3`; this should be resolved and documented against the final wiring.
- Notes mention boot/startup flow; the current portfolio scope should document startup sequencing clearly even if a separate bootloader is not part of the final implementation.

## First Commit Plan
Recommended first commit message:

`docs: add portfolio README, project plan, protocol spec, and roadmap`

Include in the first commit:

- polished `README.md`
- `docs/PROJECT_PLAN.md`
- `docs/architecture.md`
- `docs/hardware.md`
- `docs/protocol.md`
- `docs/roadmap.md`
- `docs/testing.md`
- `docs/bringup-log.md`
- `docs/troubleshooting.md`
- `docs/portfolio-summary.md`
- `docs/repository-layout.md`
- current protocol skeleton files under `Core/Inc/util` and `Core/Src/util`

Leave for later commits:

- STM32 parser integration into `iot_proc`
- Arduino cleanup or repository reorganization outside the STM32 subproject
- hardware diagrams and photos
- test evidence and captured UART logs
- power-control wrapper and final startup state machine
