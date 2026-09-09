// include this from hal-sim's driver loop to call into the proven ada/SPARK core.

// all values are in millidegrees / millidegrees-per-second, matching hal-sim's sensor_reading_t / actuator deflection units.

#ifndef FLIGHT_CORE_PITCH_CONTROLLER_H
#define FLIGHT_CORE_PITCH_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

// compute an actuator deflection command for one control loop tick; out-of-rage inputs are clamped internally and the result is proven (via GNATprove) to always
// fall within the actuator's valid rage.

int pitch_compute_command(
    int current_pitch_mdeg,
    int target_pitch_mdeg,
    int pitch_rate_mdeg_s
);

#ifdef __cplusplus
}
#endif

#endif