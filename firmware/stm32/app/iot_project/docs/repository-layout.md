# Repository Layout

## Recommended Portfolio Repository Root
```text
stm32-arduino-uart-sensor-gateway/
  README.md
  LICENSE
  docs/
  diagrams/
  images/
  firmware/
    stm32/
    arduino/
  shared/
    protocol/
  tests/
  tools/
```

## STM32 Layout
```text
firmware/stm32/
  Core/
    Inc/
      app/
      drv/
      iot/
      util/
    Src/
      app/
      drv/
      iot/
      util/
  Drivers/
  Middlewares/
  USB_HOST/
  CMakeLists.txt
```

Recommended STM32 modules:

- `app_main.c/.h`
- `sensor_gateway.c/.h`
- `uart_driver.c/.h`
- `arduino_power.c/.h`
- `system_log.c/.h`
- `uart_packet.c/.h`

## Arduino Layout
```text
firmware/arduino/sensor_node/
  sensor_node.ino
  protocol.h
  protocol.cpp
  ultrasonic_sensor.h
  ultrasonic_sensor.cpp
```

## Shared Assets
- `docs/`
  - architecture, hardware, protocol, roadmap, testing, troubleshooting
- `diagrams/`
  - block diagram, UART sequence diagram, packet format diagram
- `images/`
  - breadboard photos, logic-analyzer captures
- `tests/`
  - parser tests and validation notes
- `tools/`
  - host-side packet decoder or hex dump helpers

## Current Repository Note
The current writable workspace is the STM32 application subproject. The structure above is the recommended cleaned root layout for the final public-facing portfolio repository.
