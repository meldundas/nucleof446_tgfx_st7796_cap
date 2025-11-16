#include "dht11.h"
#include "main.h" // For HAL_GPIO functions and HAL_Delay
#include <stdio.h> // For printf

// Private function prototypes
static void DHT11_SetPinOutput(void);
static void DHT11_SetPinInput(void);
static uint8_t DHT11_ReadByte(void);
static uint8_t DHT11_CheckResponse(void);
static void DHT11_Delay_us(uint32_t us);

// Initialize DHT11 (GPIO configuration)
void DHT11_Init(void) {
    // The GPIO for DHT11 is already configured in main.c by CubeMX.
    // We just need to ensure it's set to output initially for the start signal.
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET); // Keep high initially
}

// Set DHT11 pin as output
static void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

// Set DHT11 pin as input
static void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL; // DHT11 has internal pull-up
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

// Microsecond delay function using DWT
static void DHT11_Delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t delay_cycles = us * (SystemCoreClock / 1000000U);
    while (DWT->CYCCNT - start < delay_cycles);
}


// Read a byte from DHT11
static uint8_t DHT11_ReadByte(void) {
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        // Wait for 50us low pulse (start of bit)
        uint32_t timeout = 0;
        while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET) {
            timeout++;
            if (timeout > 10000) return 0xFF; // Timeout
        }
        DHT11_Delay_us(40); // Wait for 40us (mid-point of the high pulse)
        if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) {
            data |= (1 << (7 - i)); // If pin is high, it's a '1'
        }
        timeout = 0;
        while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) { // Wait for pin to go low (end of bit)
            timeout++;
            if (timeout > 10000) return 0xFF; // Timeout
        }
    }
    return data;
}

// Check DHT11 response
static uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    DHT11_SetPinInput();
    uint32_t timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) { // Wait for DHT11 to pull low
        timeout++;
        if (timeout > 10000) return 0; // Timeout
    }
    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET) { // Wait for 80us low
        timeout++;
        if (timeout > 10000) return 0; // Timeout
    }
    timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET) { // Wait for 80us high
        timeout++;
        if (timeout > 10000) return 0; // Timeout
    }
    response = 1;
    return response;
}

// Read data from DHT11
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *data) {
    uint8_t humidity_int, humidity_dec, temp_int, temp_dec, checksum;
    uint8_t sum;

    // Request data
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET); // Pull low for 18ms
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET); // Pull high for 20-40us
    DHT11_Delay_us(30); // 30us high

    // Check response
    if (!DHT11_CheckResponse()) {
        return 0; // No response
    }

    // Read 5 bytes
    humidity_int = DHT11_ReadByte();
    humidity_dec = DHT11_ReadByte();
    temp_int = DHT11_ReadByte();
    temp_dec = DHT11_ReadByte();
    checksum = DHT11_ReadByte();

    // Check for read errors (0xFF indicates timeout)
    if (humidity_int == 0xFF || humidity_dec == 0xFF || temp_int == 0xFF || temp_dec == 0xFF || checksum == 0xFF) {
        printf("DHT11 Read Error: Timeout during byte reading.\r\n");
        return 0;
    }

    printf("DHT11 Raw Data: H_int=%d, H_dec=%d, T_int=%d, T_dec=%d, Chksum=%d\r\n",
           humidity_int, humidity_dec, temp_int, temp_dec, checksum);

    // Calculate sum and check checksum
    sum = humidity_int + humidity_dec + temp_int + temp_dec;
    if (sum == checksum) {
        // Combine 8-bit parts into 16-bit values for AM2302
        uint16_t raw_humidity = (humidity_int << 8) | humidity_dec;
        uint16_t raw_temperature = (temp_int << 8) | temp_dec;

        // Handle negative temperature for AM2302 (MSB of raw_temperature indicates sign)
        if (raw_temperature & 0x8000) { // Check if MSB is set
            data->temperature = -((float)(raw_temperature & 0x7FFF) / 10.0f);
        } else {
            data->temperature = (float)raw_temperature / 10.0f;
        }

        data->humidity = (uint8_t)((float)raw_humidity / 10.0f); // Cast to uint8_t for whole number display
        return 1; // Success
    }
    printf("DHT11 Read Error: Checksum mismatch. Calculated: %d, Received: %d\r\n", sum, checksum);
    return 0; // Checksum error
}
