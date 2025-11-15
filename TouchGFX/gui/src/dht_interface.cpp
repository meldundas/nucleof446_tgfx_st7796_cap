/*
 * dht_interface.cpp
 *
 *  Created on: 24 Jul 2024
 *      Author: mdundas
 */
#include <gui/dht_interface.h>
#include <gui/model/Model.hpp>
#include "main.h"

// C-linkage function to be called from C code
extern "C" {


    void set_backlight_value(uint8_t value)
    {
    	Displ_BackLight(value);
    }
}
