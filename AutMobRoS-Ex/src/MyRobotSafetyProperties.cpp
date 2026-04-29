#include "MyRobotSafetyProperties.hpp"
#include <eeros/logger/StreamLogWriter.hpp>

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is off"),
      slSwitchingOff("System is switching off"),
      slSwitchingOn("System is switching on"),
      slEmergency("System is in emergency state"),
      slStopping("System is stopping"),
      slSystemOn("System is on"),
      slMotorPowerOn("Motor Power is on"),

      doSystemOn("Startup the system"),
      doSystemOff("Shutdown the system"),
      switchedOff("System is switched off"),
      switchedOn("System is switched on"),
      doResetEmergency("Reset emergency"),
      stopped("Motors stopped"),
      doMotorOn("Motors on"),
      doMotorOff("Motors off"),
      doStop("Motors stopping")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    // ... = hal.getLogicOutput("...");
    greenLED = hal.getLogicOutput("onBoardLEDgreen");
    redLED = hal.getLogicOutput("onBoardLEDred");

    criticalOutputs = {greenLED, redLED};

    // Declare and add critical inputs
    // ... = eeros::hal::HAL::instance().getLogicInput("...", ...);
    buttonPause = hal.getLogicInput("onBoardButtonPause");
    buttonMode = hal.getLogicInput("onBoardButtonMode");

    criticalInputs = {buttonPause, buttonMode};

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSwitchingOff);
    addLevel(slSwitchingOn);
    addLevel(slEmergency);
    addLevel(slStopping);
    addLevel(slSystemOn);
    addLevel(slMotorPowerOn);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSwitchingOn, kPublicEvent);
    slSwitchingOff.addEvent(switchedOff, slSystemOff, kPublicEvent);
    addEventToLevelAndAbove(slSwitchingOn, doSystemOff, slSwitchingOff, kPublicEvent);
    slSwitchingOn.addEvent(switchedOn, slSystemOn, kPublicEvent);
    slEmergency.addEvent(doResetEmergency, slSystemOn, kPublicEvent);
    slStopping.addEvent(stopped, slEmergency, kPublicEvent);
    slSystemOn.addEvent(doMotorOn, slMotorPowerOn, kPublicEvent);
    slMotorPowerOn.addEvent(doMotorOff, slSystemOn, kPublicEvent);
    slMotorPowerOn.addEvent(doStop, slStopping, kPublicEvent);
    
    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({       ignore(buttonPause),                    ignore(buttonMode)});
    slSwitchingOff.setInputActions({    ignore(buttonPause),                    ignore(buttonMode)});
    slSwitchingOn.setInputActions({     ignore(buttonPause),                    ignore(buttonMode)});
    slEmergency.setInputActions({       ignore(buttonPause),                    check(buttonMode, false, doResetEmergency)});
    slStopping.setInputActions({        ignore(buttonPause),                    ignore(buttonMode)});
    slSystemOn.setInputActions({        ignore(buttonPause),                    ignore(buttonMode)});
    slMotorPowerOn.setInputActions({    check(buttonPause, false, doStop),      ignore(buttonMode)});

    // Define output actions for all levels
    // level.setOutputActions({ ... });
    slSystemOff.setOutputActions({      set(greenLED, false),    set(redLED, false)});
    slSwitchingOff.setOutputActions({   set(greenLED, true),     set(redLED, false)});
    slSwitchingOn.setOutputActions({    set(greenLED, true),     set(redLED, false)});
    slEmergency.setOutputActions({      set(greenLED, false),    set(redLED, true)});
    slStopping.setOutputActions({       set(greenLED, false),    set(redLED, true)});
    slSystemOn.setOutputActions({       set(greenLED, true),     set(redLED, false)});
    slMotorPowerOn.setOutputActions({   set(greenLED, true),     set(redLED, true)});

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        eeros::Executor::stop();
    });

    slSwitchingOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        privateContext->triggerEvent(switchedOff);
    });

    slSwitchingOn.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(switchedOn);
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
    });

    slEmergency.setLevelAction([&](SafetyContext *privateContext) {
        cs.signalChecker.reset();
    });

    slStopping.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slStopping.getNofActivations()*dt >= 1)   // wait 5 sec
        {
            privateContext->triggerEvent(stopped);
        }
    });

    slSystemOn.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slSystemOn.getNofActivations()*dt >= 1)   // wait 5 sec
        {
            privateContext->triggerEvent(doMotorOn);
        }
    });

    slMotorPowerOn.setLevelAction([&](SafetyContext *privateContext) {
        
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
