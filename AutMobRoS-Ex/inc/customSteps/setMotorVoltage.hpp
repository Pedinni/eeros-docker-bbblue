#ifndef SETMOTORVOLTAGE_HPP_
#define SETMOTORVOLTAGE_HPP_

#include <eeros/sequencer/Step.hpp>

class SetMotorVoltage : public eeros::sequencer::Step
{
public:
    SetMotorVoltage(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : eeros::sequencer::Step(name, caller),
        cs(cs)
    {
        log.info() << "Step created: " << name;
    }

    int operator() (double voltage) {
        this->voltage = voltage;
        return start();
    }

    int action()
    {
        // do something
        log.info() << "Step action executed: " << name << ", Voltage = " << voltage;
        cs.motorVoltage1.setValue(voltage);
        return 0;
    }

private:
    // Define variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
    double voltage;
};

#endif // SETMOTORVOLTAGE_HPP_
