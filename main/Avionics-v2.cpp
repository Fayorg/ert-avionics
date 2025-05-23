#include <CommandPing.h>
#include <CommandRegistry.h>
#include <Communication.h>
#include <esp_log.h>
#include <FlightState.h>
#include <NVSStore.h>
#include <RedundantStorageManager.h>
#include <SDCardStorage.h>
#include <SpiffsStorage.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <UartProcess.h>

#include <TaskRegistry.h>
#include <config/AddressConfig.h>

#include "HeartbeatTask.h"
#include <tasks/TelemetryTask.h>

#include "ApogeeTask.h"
#include "CommandEmatch.h"
#include "CommandState.h"
#include "CommandTasks.h"
#include "DetectTakeoffTask.h"
#include "UnreefedTask.h"

/*static RedundantStorageManager* manager = nullptr;
static SDCardStorage* sdCardStorage = nullptr;
static SpiffsStorage* spiffsStorage = nullptr;*/

extern "C" void app_main(void) {
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

    // NVS & Flight State Initialization
    if (NVSStore::initNVSFlash() != ESP_OK) {
        ESP_LOGE("Avionics-Init", "Failed to initialize NVS flash. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }
    #ifndef IS_GROUND_STATION
        ESP_LOGI("Avionics-Init", "Flight State is %u", FlightState::getInstance().getState());
    #endif

    // Init I2C
    ESP_LOGI("Avionics-Init", "Initializing I2C...");
    if (i2cdev_init() != ESP_OK) {
        ESP_LOGE("Avionics-Init", "Failed to initialize I2C. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }
    ESP_LOGI("Avionics-Init", "I2C initialized successfully.");

    // Testing Redundant Storage
    ESP_LOGI("Avionics-Init", "Initializing redundant storage...");
    SpiffsStorage spiffsStorage;
    SDCardStorage sdCardStorage;
    RedundantStorageManager::getInstance().init(&spiffsStorage, &sdCardStorage);

    // COMS Initialization
    ESP_LOGI("Avionics-Init", "Initializing communication...");
    if (!Communication::getInstance().init()) {
        ESP_LOGE("Avionics-Init", "Failed to initialize communication. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    #ifdef IS_GROUND_STATION
        ESP_LOGI("Avionics-Init", "Initializing peer info for ground station...");
        auto res = Communication::getInstance().init_peer_info(AVIONICS_MAC_ADDRESS_INT);
        if (!Communication::getInstance().register_receive_callback()) {
            ESP_LOGE("Avionics-Init", "Failed to register receive callback");
        }
    #else
        ESP_LOGI("Avionics-Init", "Initializing peer info for avionics...");
        auto res = Communication::getInstance().init_peer_info(GROUND_STATION_MAC_ADDRESS_INT);
        if (!Communication::getInstance().register_receive_callback()) {
            ESP_LOGE("Avionics-Init", "Failed to register receive callback");
        }
    #endif

    if (res != ESP_OK) {
        ESP_LOGW("Avionics-Init", "Failed to initialize peer info.");
    }
    ESP_LOGI("Avionics-Init", "Communication initialized successfully.");

    // Init Sensors
    #ifndef IS_GROUND_STATION
        BMP280::getInstance().init();
        MPU6050::getInstance().init();

        BMP280::getInstance().calibrate();
        MPU6050::getInstance().calibrate();
    #endif

    // Init TaskRegistry
    ESP_LOGI("Avionics-Init", "Initializing task registry...");
    // TaskRegistry::getInstance().registerTask(std::make_shared<TestTask>());
    #ifndef IS_GROUND_STATION
        TaskRegistry::getInstance().registerTask(std::make_shared<HeartbeatTask>());
        TaskRegistry::getInstance().registerTask(std::make_shared<DetectTakeoffTask>());
        TaskRegistry::getInstance().registerTask(std::make_shared<TelemetryTask>());
        TaskRegistry::getInstance().registerTask(std::make_shared<ApogeeTask>());
        TaskRegistry::getInstance().registerTask(std::make_shared<UnreefedTask>());
    #endif
    TaskRegistry::getInstance().initTasks();

    // Setting up the commands
    ESP_LOGI("Avionics-Init", "Setting up command registry & init uart");
    UartProcess::init_uart();

    CommandRegistry::getInstance().registerCommand(std::make_shared<CommandPing>());
    CommandRegistry::getInstance().registerCommand(std::make_shared<CommandTasks>());
    CommandRegistry::getInstance().registerCommand(std::make_shared<CommandState>());

    CommandRegistry::getInstance().registerCommand(std::make_shared<CommandEmatch>());

    ESP_LOGI("Avionics-Init", "Creating UART command task");
    auto status = xTaskCreate(UartProcess::wait_for_uart_command, "uart_cmd_task", 4096, NULL, 5, NULL);

    if (status != pdPASS) {
        ESP_LOGE("Avionics-Init", "Failed to create UART command task");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    if (FlightState::getInstance().getState() == FlightState::INIT) {
        FlightState::getInstance().setState(FlightState::READY);
    }
}
