#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : inverterGain(-1.0), 
      inputAccX("accX"), signalChecker(-2000, 2000),
      motorVoltage1(0.0), outputMotor1Voltage("motor1"), 
      inputEnc2("enc2"),

      x1(2.517), x2(0.2785), x3(0.01592), x4(28.723),
      outputVelocitySaturation(-15.813, 15.813), outputTorqueSaturation(-0.1, 0.1),
      motorVoltageSwitch(),

      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    inverterGain.setName("Inverter gain");
    inputAccX.setName("Input Acceleration x");
    signalChecker.setName("Signal Checker");
    motorVoltage1.setName("Motor Voltage");
    outputMotor1Voltage.setName("Output Motor 1 Voltage");
    inputEnc2.setName("Input Encoder 2");
    x1.setName("x1 gain");
    x2.setName("x2 gain");
    x3.setName("x3 gain");
    x4.setName("x4 gain");
    outputVelocitySaturation.setName("Output Velocity Saturation");
    outputTorqueSaturation.setName("Output Torque Saturation");
    motorVoltageSwitch.setName("Motor Voltage Switch");

    // Name all signals
    inputAccX.getOut().getSignal().setName("Input Acceleration x value");
    inverterGain.getOut().getSignal().setName("Inverter gain");
    motorVoltage1.getOut().getSignal().setName("Motor Voltage value");
    inputEnc2.getOut().getSignal().setName("Encoder 2 value");
    x1.getOut().getSignal().setName("w_out");
    x2.getOut().getSignal().setName("U_mot1");
    x3.getOut().getSignal().setName("M_out");
    x4.getOut().getSignal().setName("U_mot2");
    outputVelocitySaturation.getOut().getSignal().setName("w_out_sat");
    outputTorqueSaturation.getOut().getSignal().setName("M_out_sat");
    motorVoltageSwitch.getOut().getSignal().setName("U_mot");

    // Connect signals
    signalChecker.getIn().connect(inputAccX.getOut());
    
    x1.getIn().connect(inputEnc2.getOut());
    outputVelocitySaturation.getIn().connect(x1.getOut());
    x2.getIn().connect(outputVelocitySaturation.getOut());

    x3.getIn().connect(inputEnc2.getOut());
    outputTorqueSaturation.getIn().connect(x3.getOut());
    x4.getIn().connect(outputTorqueSaturation.getOut());
    motorVoltageSwitch.getIn(0).connect(x2.getOut());
    motorVoltageSwitch.getIn(1).connect(x4.getOut());
    inverterGain.getIn().connect(motorVoltageSwitch.getOut());
    outputMotor1Voltage.getIn().connect(inverterGain.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(inputEnc2);
    timedomain.addBlock(inputAccX);
    timedomain.addBlock(signalChecker);
    timedomain.addBlock(motorVoltage1);
    timedomain.addBlock(x1);
    timedomain.addBlock(outputVelocitySaturation);
    timedomain.addBlock(x2);
    timedomain.addBlock(x3);
    timedomain.addBlock(outputTorqueSaturation);
    timedomain.addBlock(x4);
    timedomain.addBlock(motorVoltageSwitch);
    timedomain.addBlock(inverterGain);
    timedomain.addBlock(outputMotor1Voltage);
    
    
    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}