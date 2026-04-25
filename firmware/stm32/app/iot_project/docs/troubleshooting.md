# Troubleshooting

## No UART Output From STM32
- confirm the configured UART instance matches the physical pins
- confirm baud rate and terminal settings
- confirm `printf` redirection targets the intended UART

## No Data Received From Arduino
- check TX/RX crossover
- check common ground
- confirm Arduino is powered and has completed startup
- confirm the STM32 is listening on the same UART instance used in wiring

## Parser Never Completes A Frame
- confirm start byte is `0xAA`
- confirm length bytes match payload size
- confirm end byte is `0xBB`
- confirm the sender is not mixing text logs and binary frames on the same link

## Sensor Readings Are Unstable
- check ultrasonic sensor wiring
- verify trigger/echo timing in the Arduino code
- test against a fixed target at a known distance
- confirm the sensor supply is stable

## Arduino Does Not Respond After Power-On
- increase startup delay before the first request
- verify the MOSFET power path and regulator output
- confirm reset behavior after power gating
