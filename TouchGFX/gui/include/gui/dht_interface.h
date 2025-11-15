#ifndef DHT_INTERFACE_H
#define DHT_INTERFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void update_dht_data(float temp, float hum);
void set_backlight_value(uint8_t value);

#ifdef __cplusplus
}
#endif

#endif // DHT_INTERFACE_H
