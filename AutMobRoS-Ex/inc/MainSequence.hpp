#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>
#include "customSteps/setMotorVoltage.hpp"
#include "customSequences/tiltExceededException.hpp"

class CheckTilt : public eeros::sequencer::Condition
{
public:
    // Initialise any neccessary attributes
    CheckTilt(ControlSystem &cs) : cs(cs) {}

    // Implement the validate method, return true if the monitor should fire
    bool validate() 
    { 
        tilt = cs.inputAccX.getOut().getSignal().getValue();
        tilt = abs(tilt);
        return tilt > max_tilt;
    }

private:
    ControlSystem &cs;
    double tilt;
    const double max_tilt = 1000.0;
};

class MainSequence : public eeros::sequencer::Sequence
{
public:
    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 MyRobotSafetyProperties &sp, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),
          setMotorVoltage("set Motor Voltage", this, cs),

          checkTilt(cs),
          tiltMonitor("tilt monitor", this, checkTilt,
                        eeros::sequencer::SequenceProp::resume, &tiltExceededException),
          tiltExceededException("max tilt angle exceeded", this, cs),

          sleep("Sleep", this)
    {
        addMonitor(&tiltMonitor);
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running)
        {
            // setMotorVoltage(1.0);
            // sleep(1.0);
            // setMotorVoltage(-1.0);
            sleep(1.0);
            log.info() << cs.inputEnc1.getOut().getSignal();
            // log.info() << cs.inputAccX.getOut().getSignal();
        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;

    SetMotorVoltage setMotorVoltage;

    CheckTilt checkTilt;
    eeros::sequencer::Monitor tiltMonitor;
    TiltExceededException tiltExceededException;

    eeros::sequencer::Wait sleep;
};

#endif // MAINSEQUENCE_HPP_