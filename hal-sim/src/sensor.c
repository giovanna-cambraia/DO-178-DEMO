#include "sensor.h"
#include <stddef.h>

static int s_initialized = 0;
static int32_t s_pitch_mdeg = 0;
static int32_t s_pitch_rate_mdeg_s = 0;
static int32_t s_disturbance_mdeg_s = 0;
static uint32_t s_time_ms = 0;

hal_status_t sensor_init(void)
{
    s_initialized = 1;
    s_pitch_mdeg = 0;
    s_pitch_rate_mdeg_s = 0;
    s_disturbance_mdeg_s = 0;
    s_time_ms = 0;
    return HAL_OK;
}
hal_status_t sensor_read(uint32_t dt_ms, sensor_reading_t *out)
{
    if (!s_initialized) {
        return HAL_ERR_NOT_INITIALIZED;
    }

    if (out == NULL) {
        return HAL_ERR_INVALID_ARG;
    }

    // TODO: replace with real physics step; placeholder integrates rate into angle so pipeline is exercisable end to end.
    s_pitch_rate_mdeg_s += s_disturbance_mdeg_s;
    s_disturbance_mdeg_s = 0;
    s_pitch_mdeg += (s_pitch_rate_mdeg_s * (int32_t)dt_ms) / 1000;
    s_time_ms += dt_ms;

    out->pitch_millideg = s_pitch_mdeg;
    out->pitch_rate_mdeg_s = s_pitch_rate_mdeg_s;
    out->timestamp_ms = s_time_ms;

    return HAL_OK;
}

hal_status_t sensor_inject_disturbance(int32_t mdeg_s_offset)
{
    if (!s_initialized) {
        return HAL_ERR_NOT_INITIALIZED;
    }

    s_disturbance_mdeg_s += mdeg_s_offset;
    return HAL_OK;
}