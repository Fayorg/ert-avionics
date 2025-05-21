// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef UARTPROCESS_H
#define UARTPROCESS_H

#include "commands/CommandRegistry.h"
#include "config/UartConfig.h"
#include "driver/uart.h"

class UartProcess {
    public:
        static void init_uart();

        static void wait_for_uart_command(void* args);
};



#endif //UARTPROCESS_H
