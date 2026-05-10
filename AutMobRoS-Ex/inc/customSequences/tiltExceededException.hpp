#ifndef TILTEXCEEDEDEXCEPTION_HPP_
#define TILTEXCEEDEDEXCEPTION_HPP_

#include <eeros/sequencer/Sequence.hpp>

class TiltExceededException : public eeros::sequencer::Sequence
{
public:
    TiltExceededException(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, caller, true),
        cs(cs)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        // do something
        log.info() << "Tilt angle is outside of the allowed range!";
        cs.motorVoltage1.setValue(0.0);
        sleep(1.0);
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem &cs;
};

#endif // TILTEXCEEDEDEXCEPTION_HPP_
