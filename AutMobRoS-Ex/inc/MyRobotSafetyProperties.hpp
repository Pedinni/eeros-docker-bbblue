#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent switchedOff;
    eeros::safety::SafetyEvent switchedOn;
    eeros::safety::SafetyEvent doResetEmergency;
    eeros::safety::SafetyEvent stopped;
    eeros::safety::SafetyEvent doSystemOff;
    eeros::safety::SafetyEvent doMotorOn;
    eeros::safety::SafetyEvent doMotorOff;
    eeros::safety::SafetyEvent doStop;

    // Define all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSwitchingOff;
    eeros::safety::SafetyLevel slSwitchingOn;
    eeros::safety::SafetyLevel slEmergency;
    eeros::safety::SafetyLevel slStopping;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slMotorPowerOn;

private:
    // Define all critical outputs
    eeros::hal::Output<bool>* greenLED;
    eeros::hal::Output<bool>* redLED;

    // Define all critical inputs
    eeros::hal::Input<bool>* buttonPause;
    eeros::hal::Input<bool>* buttonMode;

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
