// packs current sensor + actuator + controller into a fixed-size frame; stdout as newline-delimited JSON to start, swappable for a TCP/WebSocket transport.

#ifndef HAL_SIM_TELEMETRY_H
#define HAL_SIM_TELEMETRY_H

#include <stdint.h>
#include "sensor.h"

#ifdef __cplusplus
extern "C" {
    #endif

    typedef struct {
        uint32_t timestamp_ms;
        int32_t pitch_millideg;
        int32_t pitch_rate_mdeg_s;
        int32_t target_pitch_millideg;
        int32_t actuator_cmd_mdeg;
    } telemetry_frame_t;

    // initialize the telemetry sink.
    hal_status_t telemetry_init(void);

    // emit one telemetry frame; frame must be non-null.
    hal_status_t telemetry_emit(const telemetry_frame_t *frame);


    hal_status_t telemetry_shutdown(void);

    #ifdef __cplusplus
}
#endif

#endif



