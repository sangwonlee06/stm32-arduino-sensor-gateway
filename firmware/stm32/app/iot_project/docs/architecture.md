# Architecture

## System Roles
The system uses a controller-node pattern.

- `STM32F407G-DISC1`
  - main controller
  - controls Arduino power
  - owns communication timing
  - requests and parses sensor data
- `Arduino Nano`
  - sensor node
  - reads the ultrasonic sensor
  - replies with framed measurement data

## Hardware View
```text
9V Battery
  -> 5V Regulator
    -> STM32 Board Power
    -> P-MOSFET Power Switch -> Arduino Nano Power

STM32 GPIO
  -> Power Enable Control

STM32 UART TX/RX
  <-> Arduino UART RX/TX

Arduino Digital Pins
  <-> Ultrasonic Sensor Trigger/Echo
```

## Firmware View
```text
STM32
  main
    -> peripheral init
    -> uart driver init
    -> application task start

  drv_uart
    -> interrupt RX
    -> async TX

  byte_queue
    -> RX byte buffering

  uart_packet
    -> encode
    -> validate
    -> parser state machine

  iot_proc
    -> consume bytes
    -> dispatch commands
    -> process sensor replies

Arduino
  serial receive loop
    -> parser
    -> command handling
    -> ultrasonic measurement
    -> reply packet build
```

## Design Intent
- keep transport, protocol, and application logic separate
- keep the first protocol version simple enough to debug over a serial monitor
- prefer fixed-size, explicit payloads for early bring-up
- treat power sequencing as part of system behavior, not an afterthought
