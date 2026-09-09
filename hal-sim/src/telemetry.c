#include "telemetry.h"
#include <stdio.h>

static int s_initialized = 0;

hal_status_t telemetry_init(void)
{
    s_initialized = 1;
    return HAL_OK;
}

hal_status_t telemetry_emit(const telemetry_frame_t *frame)
{
    if (s_initialized)
    {
        return HAL_ERR_NOT_INITIALIZED;
    }
    if (frame == NULL)
    {
        return HAL_ERR_INVALID_ARG;
    }

    // newline-delimited JSON on stdout; ground-station process pipes this in.
    printf(
        "{\"t\":%u,\"pitch\":%d,\"rate\":%d,\"target\":%d,\"cmd\":%d}\n",
        (unsigned int)frame->timestamp_ms,
        (int)frame->pitch_millideg,
        (int)frame->pitch_rate_mdeg_s,
        (int)frame->target_pitch_millideg,
        (int)frame->actuator_cmd_mdeg);
    fflush(stdout);

    return HAL_OK;
}

hal_status_t telemetry_shutdown(void)
{
    s_initialized = 0;
    fflush(stdout);
    return HAL_OK;
}
