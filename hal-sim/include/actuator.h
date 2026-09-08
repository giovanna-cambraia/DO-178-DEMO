// simulated control-surface actuator HAL; the flight-core writes commands here.

#ifndef HAL_SIM_ACTUATOR_H
#define HAL_SIM_ACTUATOR_H

#include <stdint.h>
#include "sensor.h"

#ifdef __cplusplus
extern "C" {
    #endif

    // actuator defletion limits 
    #define ACTUATOR_MIN_MDEG (-15000)
    #define ACTUATOR_MAX_MDEG 15000

    hal_status_t actuator_init(void);

    // apply a commanded deflection, im millidegrees; values outside [ACTUATOR_MIN_MDEG, ACTUATOR_MAX_MDEG] are clamped and HAL_ERR_INVALID_ARG is returned
    hal_status_t actuator_apply(int32_t deflection_mdeg);

    // read back the last applied (post-clamp) deflection, in millidegrees; out must be non-NULL.

    hal_status_t actuator_get_state(int32_t *out_mdeg);
    
    #ifdef __cplusplus
}
#endif

#endif