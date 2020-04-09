#include "Actuators/RotatingAxis.hpp"

using namespace Actuators;

void RotatingAxis::spinAxis(uint32_t dt_us) {
    /* Control depending on mode */
    int32_t steps_todo = 0;

    if (!currently_homing) {
        /* Get Estimated Position, Velocity and Acceleration of Axis */
        position_est = motor.getEstPosition() * config.gear_ratio;
        velocity_est = motor.getEstVelocity() * config.gear_ratio;
        acceleration_est = motor.getEstAcceleration() * config.gear_ratio;

        /* Direct Command Modes */
        if (control_mode == Actuator_Modes::Position) {
            steps_todo = stepsFromPosition(position_sp, dt_us);
        } else if (control_mode == Actuator_Modes::Velocity) {
            steps_todo = stepsFromVelocity(velocity_sp, dt_us);
        } else if (control_mode == Actuator_Modes::Torque) {
            // This one is more involved, requires incorporating current.
            steps_todo = stepsFromTorque(torque_sp, dt_us);
        }

        /* Trajectory Modes */
        else if (control_mode == Actuator_Modes::PositionTrajectory ||
                 control_mode == Actuator_Modes::VelocityTrajectory ||
                 control_mode == Actuator_Modes::TorqueTrajectory) {
            static uint32_t trajector_index = 0;

            static auto localTraj = Traj;

            if (trajector_index == 0 && Traj.cyclical) {
                /* Grab a local copy of the trajectory at 0 idx */
                auto localTraj = Traj;
            }

            int32_t next_sp = localTraj.traj_arr[trajector_index];
            trajector_index++;

            // TODO: The trajectory would have to be set at the same sample time as the actuator command ts
            // otherwise, the trajectory would have to be remaped to the implementation Ts

            /* Per Mode, Map to stepper frame, and calculate number of steps required as per
             * sampling time.
             *
             * */
            if (control_mode == Actuator_Modes::PositionTrajectory) {
                steps_todo = stepsFromPosition(next_sp, dt_us);
            } else if (control_mode == Actuator_Modes::VelocityTrajectory) {
                steps_todo = stepsFromVelocity(next_sp, dt_us);
            } else if (control_mode == Actuator_Modes::TorqueTrajectory) {
                steps_todo = stepsFromTorque(next_sp, dt_us);
            }
            if (trajector_index == localTraj.traj_arr_length) {
                trajector_index = 0;
            }
        }
    } else if (currently_homing) {
        /* Step a little */
        steps_todo = -5;  // TODO: or plus?
        /* If home reached, zero stepper position */
        if (home_reached) {
            motor.zero_absolute_step();
            home_reached = false;
            currently_homing = false;
        }
    }

    /* Execute Step and Estimate Position */
    motor.stepMotor(steps_todo);
}

int32_t RotatingAxis::stepsFromPosition(int32_t &pos, uint32_t dt_us) {
    // Map axis pos to actuator pos
    // Then apply max pos, max vel, and max acc
}

int32_t RotatingAxis::stepsFromVelocity(int32_t &vel, uint32_t dt_us) {}

int32_t RotatingAxis::stepsFromTorque(int32_t &tau, uint32_t dt_us) {}
