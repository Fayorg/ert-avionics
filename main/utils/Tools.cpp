//
// Created by maxwc on 07/04/2025.
//

#include "Tools.h"

extern "C" {
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/i2c.h>
}

void Tools::switchAxis(float& x, float& y, float& z) {
    const float temp = z;
    z = x;
    x = -y;
    y = -temp;
}



