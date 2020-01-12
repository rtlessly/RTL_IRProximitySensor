/*******************************************************************************
 * Implementation file forIR Proximity Sensor
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *
 *******************************************************************************/
#define DEBUG 0

#include <Arduino.h>
#include <RTL_StdLib.h>
#include "RTL_IRProximitySensor.h"


DEFINE_CLASSNAME(IRProximitySensor);


IRProximitySensor::IRProximitySensor(const uint8_t pin, const uint8_t sensitivity, const uint8_t mode)
{
    _state.Pin = pin;
    _state.Mode = mode;
    _state.Sensitivity = constrain(sensitivity, 1, 15);
    _state.TriggerCount = 0;
    _state.Value = NOT_TRIGGERED;
    _state.PrevReading = _state.Value;

    pinMode(_state.Pin, INPUT);
}


void IRProximitySensor::Poll()
{
    TRACE(Logger(_classname_, this) << F("Poll")  << endl); 
    
    auto prevTrigger = _state.Value;
    auto currTrigger = Read();

    // Post an event when in CONTINUOUS mode or the state has changed
    if (_state.Mode == MODE_CONTINUOUS || currTrigger != prevTrigger)
    {
        QueueEvent(PROXIMITY_EVENT, currTrigger);
    }
}


void IRProximitySensor::Reset(bool triggerState)
{
    _state.Value = triggerState;
    _state.PrevReading = _state.Value;
    _state.TriggerCount = 0;
}


bool IRProximitySensor::Read()
{
    // read the IR sensor line
    auto newReading = (digitalRead(_state.Pin) == LOW);

    TRACE(Logger(_classname_, this) << F("Read: newReading=") << newReading
                                    << F(", _state.PrevReading=") << _state.PrevReading
                                    << F(", _state.TriggerCount=") << _state.TriggerCount
                                    << F(", _state.Sensitivity=") << _state.Sensitivity
                                    << F(", _state.Value=") << _state.Value
                                    << endl);

    // Update the triggered state if trigger sensitivity is exceeded.
    // NOTE: The state may not change if the previous state was just re-triggered.
    if (newReading == _state.PrevReading)
    {
        if (++_state.TriggerCount >= _state.Sensitivity) _state.Value = newReading;
    }
    else
    {
        _state.TriggerCount = 0;
    }

    _state.PrevReading = newReading;

    TRACE(Logger(_classname_, this) << F("Read: _state.Value=") << _state.Value << endl);

    return _state.Value;
}

