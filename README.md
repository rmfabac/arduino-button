# arduino-button
A simple GPIO button library for Arduino.
This library abstracts GPIO pin state, button state and event handling. Simply declare and initialize a Button, set your hardware pin and logic via callback functions for specific button events (e.g. button press or release).

## Features
- Handles button pressed, long-pressed, released, long-press released and generic state change events.
- Debounces the button pin with configurable debounce duration.
- Defaults to use the internal pull-up resistor.

## Examples
- [ButtonCallbacks](https://github.com/rmfabac/arduino-button/blob/main/examples/ButtonCallbacks/ButtonCallbacks.ino)
