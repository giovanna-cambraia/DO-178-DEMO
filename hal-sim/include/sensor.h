#ifndef HAL_SIM_SENSOR_H
#define HAL_SIM_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    HAL_OK = 0,
    HAL_ERR_NOT_INITIALIZED = 1,
    HAL_ERR_INVALID_ARG = 2
} hal_status_t;

// angles in millidegrees (int32) to avoid floating point at the HAL boundary. 
typedef struct
{
    int32_t pitch_millideg;
    int32_t pitch_rate_mdeg_s;
    uint32_t timestamp_ms;
} sensor_reading_t;

// initialize the simulated sensor; returns HAL_OK on success. 
hal_status_t sensor_init(void);

// HAL_ERR_NOT_INITIALIZED if sensor_init() not called. 
// HAL_ERR_INVALID_ARG if out is NULL. 
hal_status_t sensor_read(uint32_t dt_ms, sensor_reading_t *out);

// inject a disturbance (e.g. simulated gust) into the simulation 
hal_status_t sensor_inject_disturbance(int32_t mdeg_s_offset);

#ifdef __cplusplus
}
#endif

#endif