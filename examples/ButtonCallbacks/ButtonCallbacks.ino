#include "Button.hpp"

// Forward declarations.
void onButtonStateChanged(const Button::State aState);
void onButtonReleased();

Button button(7);

void setup()
{
    // Function reference callbacks.
    button.setOnReleasedCallback(onButtonReleased);
    button.setOnStateChangedCallback(onButtonStateChanged);

    // Lamda callbacks.
    button.setOnPressedCallback([]()
        {
            Serial.println("OnPressed");
        }
    );
    button.setOnLongPressedCallback([]()
        {
            Serial.println("OnLongPressed");
        }
    );
    button.setOnLongPressReleasedCallback([]()
        {
            Serial.println("OnLongPressReleased");
        }
    );

    // Init the button with the hardware pin.
    button.begin();    

    Serial.begin(9600);
}

void loop()
{
    // Read the button's pin state and update the button's internal state.
    // Any registered callbacks will be invoked upon button state change.
    button.read();
}

void onButtonReleased()
{
    Serial.println("OnReleased");
}

void onButtonStateChanged(const Button::State aState)
{
    Serial.println(aState);
}