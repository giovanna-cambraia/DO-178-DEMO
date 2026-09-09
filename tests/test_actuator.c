// "unit test" evidence for the C baseline -- 
// the same property that Ada/SPARK proves statically in flight-core, here only checked at these specific sample points. 

#include <assert.h>
#include "actuator.h"

int main(void)
{
    int32_t state = 0;

    assert(actuator_init() == HAL_OK);

    assert(actuator_apply(1000) == HAL_OK);
    assert(actuator_get_state(&state) == HAL_OK);
    assert(state == 1000);

    // over max: clamped, status reports the clamp 
    assert(actuator_apply(ACTUATOR_MAX_MDEG + 5000) == HAL_ERR_INVALID_ARG);
    assert(actuator_get_state(&state) == HAL_OK);
    assert(state == ACTUATOR_MAX_MDEG);

    // under min: clamped, status reports the clamp 
    assert(actuator_apply(ACTUATOR_MIN_MDEG - 5000) == HAL_ERR_INVALID_ARG);
    assert(actuator_get_state(&state) == HAL_OK);
    assert(state == ACTUATOR_MIN_MDEG);

    return 0;
}