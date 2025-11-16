#ifndef __DHT11_H
#define __DHT11_H

#include "main.h" // For GPIO_TypeDef and GPIO_Pin_TypeDef

// DHT11 sensor data structure
typedef struct {
    float temperature;
    uint8_t humidity;
} DHT11_Data_TypeDef;

// Function prototypes
void DHT11_Init(void);
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *data);

#endif /* __DHT11_H */
