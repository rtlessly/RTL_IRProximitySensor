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
#include <RTL_Stdlib.h>
#include <EventSource.h>


class IRProximitySensor : public EventSource
{
    DECLARE_CLASSNAME;

    public: static const uint8_t DEFAULT_SENSITIVITY = 1;
    public: static const uint8_t MODE_STATECHANGE = 0;
    public: static const uint8_t MODE_CONTINUOUS  = 1;
    public: static const EVENT_ID PROXIMITY_EVENT = (EventSourceID::IRProximity | EventCode::Detect);


    /*--------------------------------------------------------------------------
    Constructors
    --------------------------------------------------------------------------*/
    public: IRProximitySensor(const uint8_t pin=2, const uint8_t sensitivity= DEFAULT_SENSITIVITY, const uint8_t mode=MODE_STATECHANGE);


    /*--------------------------------------------------------------------------
    Public methods
    --------------------------------------------------------------------------*/
    public: bool Read();
    public: void Reset();
    public: void Poll();

    
    /*--------------------------------------------------------------------------
    Internal implementation
    --------------------------------------------------------------------------*/
    private: struct
    {
        uint16_t Pin          : 5;
        uint16_t Mode         : 1;
        uint16_t Triggered    : 1;
        uint16_t LastTrigger  : 1;
        uint16_t TriggerCount : 4;
        uint16_t Sensitivity  : 4;  // Number of consecutive triggers needed to toggle state
    }
    _state;
};

#endif

