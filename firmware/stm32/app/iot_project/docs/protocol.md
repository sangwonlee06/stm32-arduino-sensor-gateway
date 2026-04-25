# Protocol

## Goal
Define a simple UART packet format that is easy to bring up, easy to debug, and strict enough to reject malformed data.

## Frame Format
```text
+--------+-----------+------+----------+------+
| Start  | Length    | CMD  | Data     | End  |
| 0xAA   | 2 bytes   | 1B   | n bytes  | 0xBB |
+--------+-----------+------+----------+------+
```

Length is little-endian and represents payload size only.

## Commands
| Direction | Value | Meaning |
|-----------|-------|---------|
| Request   | `0x01` | Sensor data request |
| Request   | `0x02` | Reboot request |
| Reply     | `0x81` | Sensor data reply |
| Reply     | `0x82` | Reboot reply |

## First-Version Payload Definitions
### `0x01` Sensor Data Request
- payload length: `0`
- payload: none

### `0x02` Reboot Request
- payload length: `0`
- payload: none

### `0x81` Sensor Data Reply
- payload length: `2`
- payload: `uint16_t distance_mm`, little-endian

### `0x82` Reboot Reply
- payload length: `1`
- payload: `uint8_t status_code`

Using `distance_mm` instead of floating-point data keeps the first version portable and easy to inspect in raw hex.

## Validation Rules
- start byte must be `0xAA`
- end byte must be `0xBB`
- command must be one of the defined command values
- payload length must not exceed configured maximum
- payload length must match command-specific expectations in the first version
- malformed frames should be rejected without crashing the parser

## Parser State Machine
Recommended byte-by-byte parser states:

1. `WAIT_START`
2. `LEN_L`
3. `LEN_H`
4. `CMD`
5. `DATA`
6. `END`

This structure supports:

- interrupt-driven receive
- queue-based buffering
- parser resynchronization after line noise or framing errors

## Error Codes
Current STM32 protocol module uses these statuses:

- `UART_PACKET_STATUS_OK`
- `UART_PACKET_STATUS_NULL`
- `UART_PACKET_STATUS_INVALID_START`
- `UART_PACKET_STATUS_INVALID_END`
- `UART_PACKET_STATUS_INVALID_LENGTH`
- `UART_PACKET_STATUS_INVALID_COMMAND`
- `UART_PACKET_STATUS_BUFFER_TOO_SMALL`

## Example Frames
### Sensor Data Request
Request current distance measurement.

```text
AA 00 00 01 BB
```

Meaning:

- `AA`: start
- `00 00`: payload length `0`
- `01`: sensor data request
- `BB`: end

### Sensor Data Reply
Example for `1234 mm` distance.

```text
AA 02 00 81 D2 04 BB
```

Meaning:

- `AA`: start
- `02 00`: payload length `2`
- `81`: sensor data reply
- `D2 04`: `1234` decimal in little-endian
- `BB`: end

## Future Improvements
- add parser timeout handling
- add explicit sequence or request ID if command concurrency becomes necessary
- add CRC or checksum if the UART link proves noisy in practice
