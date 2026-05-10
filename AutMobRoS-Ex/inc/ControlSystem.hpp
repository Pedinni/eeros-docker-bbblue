#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/SignalChecker.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/Saturation.hpp>
#include <eeros/control/Switch.hpp>

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    Constant<> myConstant;
    Gain<> inverterGain;
    PeripheralInput<> inputAccX;
    PeripheralInput<> inputEnc2;
    SignalChecker<> signalChecker;
    Constant<> motorVoltage1;
    PeripheralOutput<> outputMotor1Voltage;
    Gain<> x1;
    Gain<> x2;
    Gain<> x3;
    Gain<> x4;
    Saturation<> outputVelocitySaturation;
    Saturation<> outputTorqueSaturation;
    Switch<2> motorVoltageSwitch;

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP