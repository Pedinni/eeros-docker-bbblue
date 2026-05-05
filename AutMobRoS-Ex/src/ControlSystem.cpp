#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : inverterGain(-1.0), 
      inputAccX("accX"), signalChecker(-2000, 2000),
      motorVoltage(0.0), outputMotor1Voltage("motor1"), outputMotor2Voltage("motor2"),
      inputEnc1("enc1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    inverterGain.setName("Inverter gain");
    inputAccX.setName("Input Acceleration x");
    signalChecker.setName("Signal Checker");
    motorVoltage.setName("Motor Voltage");
    outputMotor1Voltage.setName("Output Motor 1 Voltage");
    outputMotor2Voltage.setName("Output Motor 2 Voltage");
    inputEnc1.setName("Input Encoder 1");

    // Name all signals
    inputAccX.getOut().getSignal().setName("Input Acceleration x value");
    inverterGain.getOut().getSignal().setName("Inverter gain");
    motorVoltage.getOut().getSignal().setName("Motor Voltage value");
    inputEnc1.getOut().getSignal().setName("Encoder 1 value");

    // Connect signals
    signalChecker.getIn().connect(inputAccX.getOut());
    inverterGain.getIn().connect(motorVoltage.getOut());
    outputMotor1Voltage.getIn().connect(inverterGain.getOut());
    outputMotor2Voltage.getIn().connect(motorVoltage.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(inputAccX);
    timedomain.addBlock(signalChecker);
    timedomain.addBlock(motorVoltage);
    timedomain.addBlock(inverterGain);
    timedomain.addBlock(outputMotor1Voltage);
    timedomain.addBlock(outputMotor2Voltage);
    timedomain.addBlock(inputEnc1);
    
    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}