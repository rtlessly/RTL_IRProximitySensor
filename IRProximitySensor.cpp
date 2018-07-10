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
#include "IRProximitySensor.h"


DEFINE_CLASSNAME(IRProximitySensor);


IRProximitySensor::IRProximitySensor(const uint8_t pin, const uint8_t sensitivity, const uint8_t mode)
{
    _id = "IRProximitySensor";
    _state.Pin = pin;
    _state.Mode = mode;
    _state.Sensitivity = sensitivity;
    _state.TriggerCount = 0;
    _state.Triggered = false;
    _state.LastTrigger = false;

    pinMode(_state.Pin, INPUT);
}


void IRProximitySensor::Poll()
{
    TRACE(Logger(_classname_, this) << F("Poll")  << endl); 
    
    auto prevTrigger = _state.Triggered;
    auto currTrigger = Read();

    // Post an event when in CONTINUOUS mode or the state has changed
    if (_state.Mode == MODE_CONTINUOUS || currTrigger != prevTrigger)
    {
        QueueEvent(PROXIMITY_EVENT, currTrigger);
        //DispatchEvent(PROXIMITY_EVENT, _state.Triggered);
    }
}


void IRProximitySensor::Reset()
{
    _state.Triggered = false;
    _state.LastTrigger = false;
    _state.TriggerCount = 0;
}


bool IRProximitySensor::Read()
{
    // read the IR sensor line
    bool newTrigger = (digitalRead(_state.Pin) == LOW);
    bool lastTrigger = _state.LastTrigger;

    _state.TriggerCount = (newTrigger == lastTrigger) ? (_state.TriggerCount + 1) : 0;
    TRACE(Logger(_classname_, this) << F("Read: newTrigger=") << newTrigger 
                                    << F(", lastTrigger=") << lastTrigger 
                                    << F(", _state.TriggerCount=") << _state.TriggerCount 
                                    << F(", _state.Sensitivity=") << _state.Sensitivity 
                                    << endl);
    _state.LastTrigger = newTrigger;

    // Update the state if trigger sensitivity is exceeded.
    // NOTE: The state may not change if the previous state was just re-triggered.
    if (_state.TriggerCount >= _state.Sensitivity)
    {
        TRACE(Logger(_classname_, this) << F("Read: Sensitivity triggered") << endl);
        _state.Triggered = newTrigger;
        _state.TriggerCount = 0;
    }

    TRACE(Logger(_classname_, this) << F("Read: Triggered=") << _state.Triggered << endl);
    return _state.Triggered;
}
