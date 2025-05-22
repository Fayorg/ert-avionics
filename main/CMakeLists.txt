idf_component_register(
        SRCS
            "Avionics-v2.cpp"

            "state/FlightState.cpp"

            "store/NVSStore.cpp"
            "store/SpiffsStorage.cpp"
            "store/SDCardStorage.cpp"
            "store/RedundantStorageManager.cpp"

            "commands/CommandRegistry.cpp"
            "commands/CommandPing.cpp"
            "commands/UartProcess.cpp"

            "coms/Communication.cpp"
            "coms/PacketAssembler.cpp"

            "tasks/HeartbeatTask.cpp"
            "tasks/TaskRegistry.cpp"

        INCLUDE_DIRS
            "."
            "state"
            "store"
            "commands"
            "coms"
            "tasks"

        REQUIRES
            driver
            freertos
            nvs_flash
            spiffs
            fatfs
            sdmmc
            spi_flash
            esp_wifi
)
