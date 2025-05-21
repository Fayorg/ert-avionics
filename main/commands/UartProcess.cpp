// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "UartProcess.h"

void UartProcess::init_uart() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_driver_install(UART_NUM_0, UART_RX_BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void UartProcess::wait_for_uart_command(void* args) {
    char* buffer = (char*)malloc(UART_RX_BUF_SIZE);
    if (!buffer) {
        printf("Failed to allocate UART buffer\n");
        vTaskDelete(NULL);
        return;
    }

    while (true) {
        int len = uart_read_bytes(UART_NUM_0, (uint8_t*)buffer, UART_RX_BUF_SIZE - 1, pdMS_TO_TICKS(1000));
        if (len > 0) {
            buffer[len] = '\0';  // Null-terminate
            CommandRegistry::getInstance().executeCommand(std::string(buffer));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  // Slight delay to yield CPU
    }
}