// drives the simulation loop: read sensor -> compute command (Ada/SPARK core, via pitch_compute_command) -> apply to actuatpr -> emit telemetry.

#include <stdint.h>
#include <stdio.h>
#include "sensor.h"
#include "actuator.h"
#include "telemetry.h"

#ifndef PITCH_COMPUTE_COMMAND_STUB
#include "pitch_controller.h"

#else
static int pitch_compute_command(int current_pitch_mdeg, int target_pitch_mdeg, int pitch_rate_mdeg_s)
{
    // naive placeholder P-controller, replaced once flight-core links.
    int error = target_pitch_mdeg - current_pitch_mdeg;
    int cmd = (error / 2) - pitch_rate_mdeg;
    if (cmd < ACTUATOR_MIN_MDEG)
        cmd = ACTUATOR_MIN_MDEG;
    if (cmd > ACTUATOR_MAX_MDEG)
        cmd = ACTUATOR_MAX_MDEG;
    return cmd;
}
#endif

#define TARGET_PITCH_MDEG 5000
#define STEP_MS 100
#define NUM_STEPS 200

int main(void)
{
    sensor_init();
    actuator_init();
    telemetry_init();
    sensor_inject_disturbance(200);

    fprintf(stderr, "entering loop\n");

    for (int i = 0; i < NUM_STEPS; i++)
    {
        sensor_reading_t reading;
        hal_status_t status = sensor_read(STEP_MS, &reading);
        fprintf(stderr, "iteration %d, sensor_read status = %d\n", i, status);
        if (status != HAL_OK)
        {
            break;
        }

        int cmd = pitch_compute_command(
            reading.pitch_millideg,
            TARGET_PITCH_MDEG,
            reading.pitch_rate_mdeg_s);

        actuator_apply((int32_t)cmd);

        int32_t applied = 0;
        actuator_get_state(&applied);

        telemetry_frame_t frame;
        frame.timestamp_ms = reading.timestamp_ms;
        frame.pitch_millideg = reading.pitch_millideg;
        frame.pitch_rate_mdeg_s = reading.pitch_rate_mdeg_s;
        frame.target_pitch_millideg = TARGET_PITCH_MDEG;
        frame.actuator_cmd_mdeg = applied;

        hal_status_t emit_status = telemetry_emit(&frame);
        fprintf(stderr, "telemetry_emit status = %d\n", emit_status);
    }

    telemetry_shutdown();
    return 0;
}