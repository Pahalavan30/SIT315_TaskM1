# SIT315_TaskM1
Comprehensive Interrupt-Based Sense-Think-Act System


Project Summary

The project achieves an interrupt-based Arduino system using:

- PCI to sense button press on pin D8

- The ultrasonic sensor to detect objects at a 20cm range

- Timer1 interrupts to blink LED1 every 1 second

- Non-blocking logic through `millis()` to blink LED2 as and when required

 System Behavior
| Component        | Description                                     |
|------------------|-------------------------------------------------|
| LED1 (D6)        | Blinks every 1 second (Timer1 interrupt)        |
| LED2 (D4)        | Blinks when: object < 20cm OR button pressed    |
| HC-SR04 (D9/D10) | Measuring distance to objects in front of it    |
| Button (D8)      | Triggers the blinking of LED2 via PCI interrupt |

Usage:

1. Use the wiring as observed on the breadboard photo/circuit diagram.

2. Upload the `.ino` file to Arduino.

3. Open Serial Monitor (`9600 baud`) to see the updates in real time.

4. Pass an object near the sensor OR press the button to blink LED2.

Files Included
- `TaskM1.cpp` – Final Arduino code
- `circuit_photo.png` – Photo or diagram of breadboard
- `serial_output.png` – Capture of Serial Monitor output
- `README.txt` – This document
- `Reflection_Report.txt` – 300–500 word report
