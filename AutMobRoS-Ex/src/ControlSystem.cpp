#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : myConstant(1.0), myGain(2.0), 
      inputAccX("accX"), signalChecker(-2000, 2000),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    myConstant.setName("My constant");
    myGain.setName("My gain");
    inputAccX.setName("Input Acceleration x");
    signalChecker.setName("Signal Checker");

    // Name all signals
    myConstant.getOut().getSignal().setName("My constant value");
    myGain.getOut().getSignal().setName("My constant value multiplied with my gain");
    inputAccX.getOut().getSignal().setName("Input Acceleration x value");

    // Connect signals
    myGain.getIn().connect(myConstant.getOut());
    signalChecker.getIn().connect(inputAccX.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(myConstant);
    timedomain.addBlock(myGain);
    timedomain.addBlock(inputAccX);
    timedomain.addBlock(signalChecker);
    
    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}