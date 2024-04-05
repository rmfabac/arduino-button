#include "Button.hpp"


//----------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------

static constexpr int8_t DEFAULT_DEBOUNCE_DURATION = 120;      // Default duration of time (ms) to debounce pin changes.
static constexpr int16_t DEFAULT_LONG_PRESS_THRESHOLD = 1000; // Default time (ms) threshold to determine a long press.
static constexpr int8_t DEFAULT_PIN_STATE = LOW;              // Default state of the hardware pin.
static constexpr int8_t INVALID_PIN_NUMBER = -1;              // Invalid pin number.

//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------

// Default constructor.
Button::Button()
    : mDebounceDuration(DEFAULT_DEBOUNCE_DURATION)
    , mLongPressThreshold( DEFAULT_LONG_PRESS_THRESHOLD )
    , mOnLongPressedCallback(nullptr)
    , mOnLongPressReleasedCallback(nullptr)
    , mOnPressedCallback(nullptr)
    , mOnReleasedCallback(nullptr)
    , mOnStateChangedCallback(nullptr)
    , mPinNumber(INVALID_PIN_NUMBER)
    , mPinState(DEFAULT_PIN_STATE)
    , mState(StateReleased)
{
    // Intentionally left blank.
}

// Alternate constructor.
Button::Button
    (
    const uint8_t aPinNumber // Pin number to set.
    )
    : mDebounceDuration(DEFAULT_DEBOUNCE_DURATION)
    , mLongPressThreshold( DEFAULT_LONG_PRESS_THRESHOLD )
    , mOnLongPressedCallback(nullptr)
    , mOnLongPressReleasedCallback(nullptr)
    , mOnPressedCallback(nullptr)
    , mOnReleasedCallback(nullptr)
    , mOnStateChangedCallback(nullptr)
    , mPinNumber(aPinNumber)
    , mPinState(DEFAULT_PIN_STATE)
    , mState(StateReleased)
{
    begin();
}

// Configures the assigned pin to behave as an input, also enabling
// the internal pull-up resistor.
//
// NOTE: A valid pin number must be set for this function to succeed.
void Button::begin() const
{
    if (mPinNumber == INVALID_PIN_NUMBER)
    {
        return;
    }

    pinMode(mPinNumber, INPUT_PULLUP);
}

// Gets the debounce duration (ms).
uint8_t Button::getDebounceDuration() const
{
    return mDebounceDuration;
}

// Gets the long press threshold (ms).
uint8_t Button::getLongPressThreshold() const
{
    return mLongPressThreshold;
}

// Gets the pin number.
uint8_t Button::getPinNumber() const
{
    return mPinNumber;
}

// Gets the state of this button.
Button::State Button::getState() const
{
    return mState;
}

// Reads the current state of the assigned pin and sets the button
// state accordingly.
//
// NOTE: Performs debouncing.
void Button::read()
{
    // Nothing to do here if the pin number is invalid.
    if (mPinNumber == INVALID_PIN_NUMBER)
    {
        return;
    }

    const uint8_t newPinState = digitalRead(mPinNumber);
    const unsigned long currentTime = millis();

    if (mPinState != newPinState)
    {
        // Debounce the signal.
        if (currentTime - mLastPinStateChangeTime > mDebounceDuration)
        {
            mPinState = newPinState;
            mLastPinStateChangeTime = currentTime;
            
            if (mPinState == LOW)
            {
                setState(StatePressed);
            }
            else
            {
                setState(StateReleased);
            }
        }
    }
    // Check for long press event.
    else if (mState == StatePressed &&
            currentTime - mLastPressedStateChangeTime > mLongPressThreshold)
    {
        // TODO: Rethink this implementation when adding support for ISR. We need
        // a mechanism for checking long press events without relying on pin state changes
        // (e.g. a public "tick()" function could be called in the main loop).
        setState(StateLongPressed);
    }
}

// Sets the debounce duration .
void Button::setDebounceDuration
    (
    const uint8_t aDuration // Duration to set (ms).
    )
{
    mDebounceDuration = aDuration;
}

// Sets the on long press threshold.
void Button::setLongPressThreshold
    (
    const uint8_t aThreshold // Threshold to set (ms).
    )
{
    mLongPressThreshold = aThreshold;
}

// Sets the on long pressed callback.
void Button::setOnLongPressedCallback
    (
    const Callback& aCallback // Callback to set.
    )
{
    mOnLongPressedCallback = aCallback;
}

// Sets the on long press released callback.
void Button::setOnLongPressReleasedCallback
    (
    const Callback &aCallback // Callback to set.
    )
{
    mOnLongPressReleasedCallback = aCallback;
}

// Sets the on pressed callback.
void Button::setOnPressedCallback
    (
    const Callback& aCallback // Callback to set.
    )
{
    mOnPressedCallback = aCallback;
}

// Sets the on released callback.
void Button::setOnReleasedCallback
    (
    const Callback& aCallback // Callback to set.
    )
{
    mOnReleasedCallback = aCallback;
}

// Sets the on state changed callback.
void Button::setOnStateChangedCallback
    (
    const StateChangeCallback& aCallback // Callback to set.
    )
{
    mOnStateChangedCallback = aCallback;
}

// Sets the pin number.
void Button::setPinNumber
    (
    const uint8_t aPinNumber // Pin number to set.
    )
{
    mPinNumber = aPinNumber;
}

// Compares this button to another.
bool Button::operator==
    (
    const Button &rhs //!< Button to compare.
    )
{
    return (this == &rhs);
}

//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------

// Notifies listeners of a button long press.
void Button::notifyLongPressed() const
{
    if (mOnLongPressedCallback != nullptr)
    {
        mOnLongPressedCallback();
    }
}

// Notifies listeners of a button long press release.
void Button::notifyLongPressReleased() const
{
    if (mOnLongPressReleasedCallback != nullptr)
    {
        mOnLongPressReleasedCallback();
    }
}

// Notifies listeners of a button press.
void Button::notifyPressed() const
{
    if (mOnPressedCallback != nullptr)
    {
        mOnPressedCallback();
    }
}

// Notifies listeners of a button release.
void Button::notifyReleased() const
{
    if (mOnReleasedCallback != nullptr)
    {
        mOnReleasedCallback();
    }
}

// Notifies listeners of a state change.
void Button::notifyStateChange() const
{
    if (mOnStateChangedCallback != nullptr)
    {
        mOnStateChangedCallback(mState);
    }
}

// Sets the button state and notifies listeners.
void Button::setState
    (
    const State aState // State to set.
    )
{
    if (mState != aState)
    {
        if (aState == StatePressed)
        {
            mLastPressedStateChangeTime = millis();
            notifyPressed();
        }
        else if (aState == StateLongPressed)
        {
            notifyLongPressed();
        }
        else if (aState == StateReleased)
        {
            if (mState != StateLongPressed)
            {
                notifyReleased();
            }
            else
            {
                notifyLongPressReleased();
            }
        }

        mState = aState;
        notifyStateChange();
    }
}
