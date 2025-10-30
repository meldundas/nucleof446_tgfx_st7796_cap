#include "ft6336.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c1;

void ft6336_init(void)
{
    // The I2C peripheral is already initialized in main.c by MX_I2C1_Init()
    // We can add any specific FT6336 initialization here if needed.
    // For now, we just check if the device is present.
    uint8_t device_id = 0;
    device_id = ft6336_read_byte(FT6336_ADDR, FT6336_ADDR_FIRMARE_ID);
    if (device_id != 0x41) // Chip ID for FT6336
    {
        // Error: device not found
    }
}

unsigned char ft6336_write_byte(unsigned char addr,unsigned char reg,unsigned char data)
{
    return HAL_I2C_Mem_Write(&hi2c1, addr << 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

unsigned char ft6336_read_byte(unsigned char addr,unsigned char reg)
{
    unsigned char res;
    HAL_I2C_Mem_Read(&hi2c1, addr << 1, reg, I2C_MEMADD_SIZE_8BIT, &res, 1, 1000);
    return res;
}

unsigned char ft6336_get_td_status(void)
{
		unsigned char a;
		a=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TD_STATUS);
		return a;
}

void ft6336_get_touch1_position(unsigned int *x,unsigned int *y)
{
		unsigned int xh=0,xl=0,yh=0,yl=0;
		xh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_XH);
		xl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_XL);
		yh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_YH);
		yl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_YL);
		*x=((xh&0x000F)<<8)|xl;
		*y=((yh&0x000F)<<8)|yl;
}

void ft6336_get_touch2_position(unsigned int *x,unsigned int *y)
{
		unsigned int xh=0,xl=0,yh=0,yl=0;
		xh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_XH);
		xl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_XL);
		yh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_YH);
		yl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_YL);
		*x=((xh&0x000F)<<8)|xl;
		*y=((yh&0x000F)<<8)|yl;
}

uint8_t FT6336_TouchGFXSampleTouch(int32_t *x, int32_t *y)
{
    uint8_t touch_detected = 0;
    if (ft6336_get_td_status() > 0)
    {
        unsigned int temp_x, temp_y;
        ft6336_get_touch1_position(&temp_x, &temp_y);
        *x = 480 - temp_y;
        *y = temp_x;
        touch_detected = 1;
    }
    return touch_detected;
}