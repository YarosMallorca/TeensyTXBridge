# Teensy TX Bridge

Teensy TX Bridge is a firmware that allows you to control RC transmitters via the Trainer port using PPM, over a USB Serial connection.

This allows you to make your own control scheme for your aircraft, by sending commands from a computer in real-time.
This can be used, for example, to control an aircraft using a non-standard controller such as a flight simulator setup, or a custom-built controller.

> [!CAUTION]
> This project involves working with RC transmitters and aircraft. Improper use can lead to loss of control of the aircraft, which may result in damage or injury. Always test in a safe environment and ensure you have a way to regain control of your aircraft. **I am not responsible for any damage or injury caused by the use of this project. Use at your own risk and liability!**

## Hardware Requirements

- A Teensy board _(this is specifically designed for Teensy, I might add support for Arduinos in the future)_
- A mono or stereo 3.5mm TRS cable to connect the Teensy to the Trainer port of your RC transmitter.
- A USB cable to connect the Teensy to your computer.

## Wiring Diagram

Cut off one side of the 3.5mm TRS cable and strip the wires, then connect them to the Teensy as follows:

| TRS Pin | Teensy Pin | Function          |
| ------- | ---------- | ----------------- |
| Sleeve  | GND        | Ground            |
| Tip     | PPM_PIN    | PPM Signal Output |

Valid pins by board type:

```txt
Teensy 3.6, 3.5:  5, 6, 9, 10, 20, 21, 22, 23
Teensy 3.2, 3.1:  5, 6, 9, 10, 20, 21, 22, 23
Teensy LC:        6, 9, 10, 20, 22, 23
Teensy 4.0:       6, 9, 10, 11, 12, 13, 14, 15, 18, 19
```

## Protocol

The Teensy listens for commands over the USB Serial connection at a specified baud (115200 by default).

These are the available commands:

| Command                 | Description                                          |
| ----------------------- | ---------------------------------------------------- |
| `CH:1500,1500,1000,...` | Set channel pulse widths (up to 16, comma-separated) |
| `FS`                    | Force failsafe immediately (manual trigger)          |
| `RST`                   | Reset failsafe and resume normal operation           |

In the firmware, a default failsafe configuration and timeout are configured. If no valid channel data is received within the timeout period (500ms by default), or if the `FS` command is received, the Teensy will output the failsafe channel values.

To reset the failsafe, and resume normal operation, send the `RST` command.
Always send this command in the beginning of your client script, after connecting to the Teensy and when your control source is ready to send valid channel data.

## Software Setup

- Install the [Arduino IDE](https://www.arduino.cc/en/software/) (version 2.0 or later)
- Install the [Teensyduino add-on](https://www.pjrc.com/teensy/td_download.html) for Arduino IDE
- Open the `RC.ino` file in the Arduino IDE
- Select the board in Tools > Board > Teensy
- Select the correct USB type in Tools > USB Type > Serial
- Set Optimize in Tools > Optimize > Fastest
- Adjust any of the following parameters in `RC.ino` as needed:

  - `PPM_PIN`: The pin used for PPM output (default 23)
  - `MAX_CHANNELS`: The number of channels to output (default 16, max 16)
  - `SERIAL_BAUD`: The baud rate for the Serial connection (default 115200)
  - `SERIAL_TIMEOUT`: The timeout in milliseconds before triggering failsafe if no serial data is received (default 500)
  - `failsafeChannels[]`: The default failsafe channel values (modify as needed)

- Upload the code to the Teensy board

## Usage

- Connect the Teensy to your computer via USB
- Connect the Teensy to the Trainer port of your RC transmitter using the 3.5mm TRS cable
- On your radio, turn on the Trainer mode (in EdgeTX, this is under Model Setup > Trainer > Mode: "Master/Jack"; then adding a Special Function "Trainer" to activate the Trainer output)
- Use a script of your choice to send commands over the Serial connection to control the channels

## Acknowledgements

- [PulsePosition Library](https://github.com/PaulStoffregen/PulsePosition) by Paul Stoffregen for generating PPM signals
