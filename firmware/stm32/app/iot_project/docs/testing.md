# Testing

## Testing Strategy
Testing should move from isolated checks to full-system validation.

## Bring-Up Tests
### STM32
- flash and run known-good firmware
- verify GPIO output on an LED or test pin
- verify UART transmit through terminal or logic analyzer
- verify UART receive interrupt path with loopback

### Arduino
- verify serial echo
- verify ultrasonic sensor trigger/echo timing
- verify repeatable distance output at known ranges

## Integration Tests
- verify common ground between STM32 and Arduino
- verify UART exchange at `115200`
- verify packet request from STM32 and reply from Arduino
- verify parser rejects malformed frames and recovers on the next valid start byte

## Power Sequencing Tests
- verify STM32 can disable and enable Arduino power
- measure or observe Arduino startup delay after re-power
- verify behavior when STM32 sends too early
- verify timeout behavior when Arduino does not respond

## Negative Tests
- wrong baud rate
- reversed TX/RX
- missing ground
- truncated frame
- invalid start byte
- invalid end byte
- unsupported command value
- oversized payload length

## Evidence To Capture
- UART terminal logs
- logic-analyzer captures if available
- photos of the hardware setup
- short bring-up notes for each milestone

## Exit Criteria For Initial Portfolio Version
- STM32 can request sensor data
- Arduino can return a valid framed reply
- STM32 can parse and display the distance value
- repository includes architecture, protocol, and testing documentation
