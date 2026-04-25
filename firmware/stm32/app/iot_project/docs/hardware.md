# Hardware Notes

## Boards And Devices
- main controller: `STM32F407G-DISC1`
- sensor node: `Arduino Nano`
- sensor: ultrasonic/sonar module
- supply: `9V` battery with `5V` regulator
- power switch: P-MOSFET controlled by STM32 GPIO

## Intended Power Flow
1. `9V` battery supplies the system input.
2. A regulator provides a `5V` rail.
3. The STM32 controls whether the Arduino receives power through the switch stage.
4. The Arduino powers the ultrasonic sensor and performs the local measurement.

## Intended Signal Flow
1. STM32 enables Arduino power.
2. STM32 waits for startup stabilization.
3. STM32 sends a UART request.
4. Arduino reads the sensor and sends a UART reply.
5. STM32 parses and uses the returned data.

## Wiring Assumptions
- UART TX/RX are crossed correctly between boards
- both boards share a common ground
- logic-level compatibility is confirmed in the final wiring
- ultrasonic trigger and echo are attached to Arduino digital pins
- original notes suggest pins near `D3/D4`, but final wiring should be documented with exact pin names

## Hardware Risks To Validate
- incorrect UART wiring or missing common ground
- unstable Arduino startup timing after gated power-on
- voltage mismatch on UART pins
- noisy power or regulator dropout under load
- unreliable ultrasonic readings due to sensor placement or timing
