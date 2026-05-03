#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>
#include "customSteps/setMotorVoltage.hpp"

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
          sleep("Sleep", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        while (eeros::sequencer::Sequencer::running)
        {
            setMotorVoltage(1.0);
            sleep(1.0);
            setMotorVoltage(-1.0);
            sleep(1.0);
            // log.info() << cs.inverterGain.getOut().getSignal();
            log.info() << cs.inputAccX.getOut().getSignal();
        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;

    SetMotorVoltage setMotorVoltage;

    eeros::sequencer::Wait sleep;
};

#endif // MAINSEQUENCE_HPP_