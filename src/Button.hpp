//------------------------------------------------------------------------------
//! @file Button.hpp
//!
//! This file contains declarations for the Button class.
//------------------------------------------------------------------------------
#ifndef Button_hpp
#define Button_hpp

#if (defined(ARDUINO_ARCH_ESP32) || defined(ESP8266))
{
    #define FUNCTIONAL_SUPPORT
}
#endif

#ifdef FUNCTIONAL_SUPPORT
{
    #include <functional>
}
#endif

#include <Arduino.h>
#include <stdint.h>


//! This class abstracts hardware button handling, notifying listeners of
//! button events (e.g. a button press).
class Button
{
public:
    //----------------------------------------------------------------------
    // Public Enumerations
    //----------------------------------------------------------------------

    //! Button states.
    enum State
    {
        StateReleased,   //!< Button released state.
        StatePressed,    //!< Button pressed state.
        StateLongPressed //!< Button long pressed state.
    };

    //----------------------------------------------------------------------
    // Public Types
    //----------------------------------------------------------------------

    #ifdef FUNCTIONAL_SUPPORT
        typedef std::function<void()> Callback;
        typedef std::function<void(const State)> StateChangeCallback;
    #else
        typedef void (*Callback)();
        typedef void (*StateChangeCallback)(const State);
    #endif

    //----------------------------------------------------------------------
    // Public Functions
    //----------------------------------------------------------------------

    Button();

    Button(const uint8_t aPinNumber);

    virtual ~Button() = default;

    virtual void begin() const;

    uint8_t getDebounceDuration() const;

    uint8_t getLongPressThreshold() const;

    uint8_t getPinNumber() const;

    State getState() const;

    virtual void read();

    void setDebounceDuration(const uint8_t aDuration);

    void setLongPressThreshold(const uint8_t aThreshold);

    void setOnLongPressedCallback(const Callback& aCallback);

    void setOnLongPressReleasedCallback(const Callback& aCallback);

    void setOnPressedCallback(const Callback& aCallback);

    void setOnReleasedCallback(const Callback& aCallback);

    void setOnStateChangedCallback(const StateChangeCallback& aCallback);

    void setPinNumber(const uint8_t aPinNumber);

    bool operator==(const Button& rhs);
protected:
    //----------------------------------------------------------------------
    // Protected Variables
    //----------------------------------------------------------------------

    unsigned long mLastPinStateChangeTime; //!< Time of last valid (debounced) pin state change.
    
    unsigned long mLastPressedStateChangeTime; //!< Time of last pressed state change.
    
    uint8_t mPinNumber; //!< Assigned pin number of the hardware button.
    
    uint8_t mPinState; //!< Current state of the hardware pin (HIGH/LOW).
    
    State mState; //!< Current state of the button.

private:
    //----------------------------------------------------------------------
    // Private Functions
    //----------------------------------------------------------------------

    void notifyLongPressed() const;

    void notifyLongPressReleased() const;

    void notifyPressed() const;

    void notifyReleased() const;

    void notifyStateChange() const;

    void setState(const State aState);

    //----------------------------------------------------------------------
    // Private Variables
    //----------------------------------------------------------------------

    uint8_t mDebounceDuration; //!< Duration of time (ms) to debounce pin changes.
    
    uint16_t mLongPressThreshold; //!< Time (ms) threshold to determine a long press.
    
    // TODO: Implement external interrupt (ISR) handling.
    // Callback mOnInterruptCallback; //!< Callback invoked during an external interrupt.
   
    Callback mOnLongPressedCallback; //!< Callback invoked during a long press event.
    
    Callback mOnLongPressReleasedCallback; //!< Callback invoked during a long press release event.
    
    Callback mOnPressedCallback; //!< Callback invoked during a press event.
    
    Callback mOnReleasedCallback; //!< Callback invoked during a release event.
    
    StateChangeCallback mOnStateChangedCallback; //!< Callback invoked during a click event.
};
#endif // Button_hpp