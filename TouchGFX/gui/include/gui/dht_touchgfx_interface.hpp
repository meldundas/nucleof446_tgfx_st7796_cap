#ifndef DHT_TOUCHGFX_INTERFACE_HPP
#define DHT_TOUCHGFX_INTERFACE_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // For uint8_t

void updateTouchGFXSensorData(float temp, uint8_t humidity);

#ifdef __cplusplus
}
#endif

#endif // DHT_TOUCHGFX_INTERFACE_HPP