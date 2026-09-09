#include "actuator.h"
#include <stddef.h>

static int s_initialized = 0;
static int32_t s_last_applied_mdeg = 0;

hal_status_t actuator_init(void)
{
    s_initialized = 1;
    s_last_applied_mdeg = 0;
    return HAL_OK;
}

hal_status_t actuator_apply(int32_t deflection_mdeg)
{
    hal_status_t status = HAL_OK;
    int32_t clamped = deflection_mdeg;

    if (!s_initialized) {
        return HAL_ERR_NOT_INITIALIZED;
    }

    if (clamped < ACTUATOR_MIN_MDEG) {
        clamped = ACTUATOR_MIN_MDEG;
        status = HAL_ERR_INVALID_ARG;
    } else if (clamped > ACTUATOR_MAX_MDEG) {
        clamped = ACTUATOR_MAX_MDEG;
        status = HAL_ERR_INVALID_ARG;
    } else {
        // value already within bounds
    }

    s_last_applied_mdeg = clamped;
    return status;
}

hal_status_t actuator_get_state(int32_t *out_mdeg)
{
    if (!s_initialized) {
        return HAL_ERR_NOT_INITIALIZED;
    }

    if (out_mdeg == NULL) {
        return HAL_ERR_INVALID_ARG;
    }
    *out_mdeg = s_last_applied_mdeg;
    return HAL_OK;
}