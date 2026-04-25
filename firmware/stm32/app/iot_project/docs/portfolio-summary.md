# Portfolio Summary

## One-Paragraph Summary
`STM32-Arduino UART Sensor Gateway` is an embedded systems project that uses an STM32 microcontroller to control power to an Arduino Nano, communicate with it over UART, request ultrasonic sensor data, and parse structured binary replies. The project is designed to demonstrate practical firmware development, hardware integration, protocol design, debugging, and documentation discipline in a small but complete system.

## Why It Works As A Portfolio Project
- it involves real hardware instead of simulation only
- it shows communication between two microcontrollers
- it includes protocol design rather than ad hoc serial prints
- it creates a clear story around bring-up, debugging, and validation
- it is small enough to understand end to end during an interview

## Interview Talking Points
- why the sensor is attached to the Arduino instead of directly to STM32
- why the first protocol version uses fixed-point `distance_mm`
- how interrupt-driven UART receive feeds a queue and parser
- how power sequencing changes system timing and failure modes
- how malformed frames are handled safely
- what would be improved next, such as CRC, retries, and host-side tests
