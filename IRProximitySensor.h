/*******************************************************************************
 * Header file for IR Proximity Sensor
 *
 * created 19 Oct 2016
 * by R. Terry Lessly
 *
 *******************************************************************************/
#ifndef _IRProximitySensor_H_
#define _IRProximitySensor_H_

#include <inttypes.h>
#include <EventSource.h>


class IRProximitySensor : public EventSource
{
    public: static const uint8_t MODE_STATECHANGE = 0;
    public: static const uint8_t MODE_CONTINUOUS  = 1;
    
    public: static const EVENT_ID PROXIMITY_EVENT = (EventSourceID::IRProximity | EventCode::Detect);

    //**************************************************************
    // Constructors
    //**************************************************************
    public: IRProximitySensor(const uint8_t pin=2, const uint8_t mode=MODE_STATECHANGE);

    //**************************************************************
    // Public methods
    //**************************************************************
    public: bool Read();

    public: void Reset();

    public: virtual void Poll();

    //**************************************************************
    // Internal state
    //**************************************************************
    private: struct
    {
        uint16_t Pin          : 5;
        uint16_t Mode         : 1;
        uint16_t Triggered    : 1;
        uint16_t LastTrigger  : 1;
        uint16_t TriggerCount : 6;
    }
    _state;
};

#endif

