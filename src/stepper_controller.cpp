#include <pico/stdlib.h>
#include <robotarm/stepper_controller.h>

#include <cmath>

namespace robotarm {

const int toMax = 1984;  // TODO

StepperController::StepperController() {}

void StepperController::add_stepper(std::shared_ptr<Stepper> stepper) {
    m_steppers.push_back(stepper);
}

void StepperController::enable_all() {
    for (auto stepper : m_steppers) {
        stepper->enable();
    }
}

void StepperController::disable_all() {
    for (auto stepper : m_steppers) {
        stepper->disable();
    }
}

void StepperController::move_motors(const std::vector<double> &target_rotations, const std::vector<bool> &clockwise, const std::vector<StepperSpeed> &speeds) {
    // a combination of sleeps and stepper moves
    // therefore blocking
    // TODO: make it non-blocking and being called from the main loop iteratively
    // or add callbacks?

    std::vector<int> targetPos;
    std::vector<int> accChanges;

    for (int i = 0; i < m_steppers.size(); i++) {
        targetPos.push_back(m_steppers[i]->steps_for_rotation(target_rotations[i]));
        accChanges.push_back(motion_planning(m_steppers[i]->current_position(), targetPos[i], m_steppers[i]));
    }

    std::vector<int> accelBins;
    std::vector<int> deccelBins;
    std::vector<double> waitTimes;
    std::vector<int> roundedWaitTimes;
    std::vector<uint64_t> changeTimes;

    const int n = m_steppers.size();
    accelBins.resize(n, 0);
    deccelBins.resize(n, 0);
    waitTimes.resize(n, 0.003);
    roundedWaitTimes.resize(n, 0);
    changeTimes.resize(n, 0);

    auto micros = []() { return to_us_since_boot(get_absolute_time()); };

    for (int i = 0; i < m_steppers.size(); i++) {
        accelBins[i] = accChanges[i];
        deccelBins[i] = std::abs(targetPos[i] - m_steppers[i]->current_position()) - accChanges[i];
        waitTimes[i] = next_time(&accelBins[i], &deccelBins[i], waitTimes[i]);
        roundedWaitTimes[i] = std::round(waitTimes[i] * 1000000);
        changeTimes[i] = micros() + roundedWaitTimes[i];
    }

    int count = 0;
    while (true) {
        uint64_t current_time = micros();

        for (int i = 0; i < m_steppers.size(); i++) {
            if (current_time > changeTimes[i]) {
                m_steppers[i]->step_high();
                m_steppers[i]->step_low();
                if (accelBins[i] != 0 || deccelBins[i] != 0) {
                    waitTimes[i] = next_time(&accelBins[i], &deccelBins[i], waitTimes[i]);
                    roundedWaitTimes[i] = std::round(waitTimes[i] * 1000000);
                    changeTimes[i] = current_time + roundedWaitTimes[i];
                }
            }
        }
        // // if all motors are done, break
        // bool allDone = true;
        // for (int i = 0; i < m_steppers.size(); i++) {
        //     if (accelBins[i] != 0 || deccelBins[i] != 0) {
        //         allDone = false;
        //         break;
        //     }
        // }
        // if (allDone) {
        //     break;
        // }
    }
}

void StepperController::move_motors(const std::vector<double> &target_rotations, const std::vector<bool> &clockwise, StepperSpeed speed) {
    std::vector<StepperSpeed> speeds;
    for (int i = 0; i < target_rotations.size(); i++) {
        speeds.push_back(speed);
    }
    move_motors(target_rotations, clockwise, speeds);
}

void StepperController::move_motors(const double &target_rotation, const bool &clockwise, const StepperSpeed &speed) {
    std::vector<double> target_rotations;
    std::vector<bool> clockwise_vector;
    std::vector<StepperSpeed> speeds;

    for (int i = 0; i < m_steppers.size(); i++) {
        target_rotations.push_back(target_rotation);
        clockwise_vector.push_back(clockwise);
        speeds.push_back(speed);
    }

    move_motors(target_rotations, clockwise_vector, speeds);
}

double StepperController::positive_acceleration(double waitTime) const {
    double dVelocity = waitTime * 4000;
    waitTime = 1. / (dVelocity + 1. / waitTime);
    if (waitTime < 0.00025) {
        waitTime = 0.00025;
    }
    return waitTime;
}

double StepperController::negative_acceleration(double waitTime) const {
    double dVelocity = waitTime * -4000;
    waitTime = 1. / (dVelocity + 1. / waitTime);
    if (waitTime < 0.003) {
        waitTime = 0.003;
    }
    return waitTime;
}

double StepperController::next_time(int *accelBin, int *deccelBin, double waitTime) {
    if (*accelBin != 0) {
        waitTime = positive_acceleration(waitTime);
        (*accelBin)--;
    } else if (*deccelBin != 0) {
        waitTime = negative_acceleration(waitTime);
        (*deccelBin)--;
    } else {
        waitTime = 0.0;
    }
    return waitTime;
}

int StepperController::motion_planning(int current_pos, int target_pos, std::shared_ptr<Stepper> stepper) {
    int totalSteps = std::abs(target_pos - current_pos);
    int accelChange;
    if (totalSteps > 2 * toMax) {
        accelChange = totalSteps - toMax;
    } else {
        accelChange = std::round(totalSteps / 2);
    }

    if (target_pos > current_pos) {
        stepper->set_dir(true);
    } else {
        stepper->set_dir(false);
    }

    return accelChange;
}

}  // namespace robotarm