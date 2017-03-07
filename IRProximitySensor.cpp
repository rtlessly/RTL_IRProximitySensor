/*******************************************************************************
 * Implementation file forIR Proximity Sensor
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *
 *******************************************************************************/
#define DEBUG 0

#include <Arduino.h>
#include <Debug.h>
#include "IRProximitySensor.h"

#define SENSITIVITY 8 // Number of consecutive triggers needed to set state


static DebugHelper Debug("IRProximitySensor");


EVENT_ID IRProximitySensor::PROXIMITY_EVENT = EventSource::GenerateEventID();


IRProximitySensor::IRProximitySensor(const uint8_t pin, const uint8_t mode)
{
    _state.Pin = pin;
    _state.Mode = mode;
    _state.Triggered = false;
    _state.LastTrigger = false;
    _state.TriggerCount = 0;

    pinMode(_state.Pin, INPUT);
}


void IRProximitySensor::Poll()
{
    Read();
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

    Debug.Log("Read => newTrigger=%b, lastTrigger=%b, _state.TriggerCount=%i", newTrigger, lastTrigger, _state.TriggerCount);

    _state.LastTrigger = newTrigger;

    // Update the state if trigger sensitivity is exceeded.
    // NOTE: The state may not change if the previous state was just re-triggered.
    if (_state.TriggerCount >= SENSITIVITY)
    {
        Debug.Log("Read => Sensitivity triggered");
        _state.TriggerCount = 0;

        // When in STATE_CHANGE mode only send an event when the state has changed,
        // so if the state didn't change then don't do anything (i.e., just return)
        if (_state.Mode == MODE_STATECHANGE && newTrigger == _state.Triggered) return;

        // Otherwise, send an event
        _state.Triggered = newTrigger;

        Event event(PROXIMITY_EVENT, _state.Triggered);

        DispatchEvent(&event);
    }

    Debug.Log("Read => state=", (_state.Triggered ? "TRIGGERED" : "NOT TRIGGERED"));
    return _state.Triggered;
}
