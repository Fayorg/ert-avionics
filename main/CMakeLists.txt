idf_component_register(
        SRCS
            "Avionics-v2.cpp"

            "state/FlightState.cpp"

            "store/NVSStore.cpp"
            "store/SpiffsStorage.cpp"
            "store/SDCardStorage.cpp"
            "store/RedundantStorageManager.cpp"

        INCLUDE_DIRS
            "."
            "state"
            "store"

        REQUIRES
            driver
            freertos
            nvs_flash
            spiffs
            fatfs
            sdmmc
            spi_flash
)
