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
            "commands/CommandTasks.cpp"
            "commands/CommandState.cpp"

            "coms/Communication.cpp"
            "coms/PacketAssembler.cpp"
            "coms/PacketToJson.cpp"

            "tasks/TaskRegistry.cpp"
            "tasks/HeartbeatTask.cpp"
            "tasks/TelemetryTask.cpp"
            "tasks/DetectTakeoffTask.cpp"
            "tasks/ApogeeTask.cpp"
            "tasks/UnreefedTask.cpp"

            "sensors/Sensor.cpp"
            "sensors/MPU6050.cpp"
            "sensors/BMP280.cpp"
            "sensors/HMC5883L.cpp"

            "filters/MadgwickAHRS.cpp"

            "utils/Tools.cpp"

        INCLUDE_DIRS
            "."
            "state"
            "store"
            "commands"
            "coms"
            "tasks"
            "sensors"
            "filters"
            "utils"

        REQUIRES
            driver
            freertos
            nvs_flash
            spiffs
            fatfs
            sdmmc
            spi_flash
            esp_wifi
            bmp280
            mpu6050
            qmc5883l
            i2cdev
            esp_idf_lib_helpers
            arduinojson
            esp_timer
)

option(IS_GROUND_STATION_BUILD "Build for Ground Station (defines IS_GROUND_STATION=1)" OFF)

if(IS_GROUND_STATION_BUILD)
    message(STATUS "Building for Ground Station")
    add_compile_definitions(IS_GROUND_STATION=1)
else()
    message(STATUS "Building for Avionics")
endif()
