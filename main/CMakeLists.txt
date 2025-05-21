idf_component_register(
        SRCS
            "Avionics-v2.cpp"

            "state/FlightState.cpp"

            "store/NVSStore.cpp"

        INCLUDE_DIRS
            "."
            "state"
            "store"

        REQUIRES
            driver
            freertos
            nvs_flash
)
