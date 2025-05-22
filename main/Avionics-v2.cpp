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

#include "TestTask.h"

extern "C" void app_main(void)
{
    // NVS & Flight State Initialization
    if (NVSStore::initNVSFlash() != ESP_OK) {
        ESP_LOGE("Avionics-Init", "Failed to initialize NVS flash. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    ESP_LOGI("Avionics-Init", "Flight State is %u", FlightState::getInstance().getState());

    // COMS Initialization
    ESP_LOGI("Avionics-Init", "Initializing communication...");
    if (!Communication::getInstance().init()) {
        ESP_LOGE("Avionics-Init", "Failed to initialize communication. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    // Setting up the commands
    ESP_LOGI("Avionics-Init", "Setting up command registry & init uart");
    UartProcess::init_uart();

    CommandRegistry::getInstance().registerCommand(std::make_shared<CommandPing>());

    ESP_LOGI("Avionics-Init", "Creating UART command task");
    auto status = xTaskCreate(UartProcess::wait_for_uart_command, "uart_cmd_task", 4096, NULL, 5, NULL);

    if (status != pdPASS) {
        ESP_LOGE("Avionics-Init", "Failed to create UART command task");
    }

    // Init TaskRegistry
    ESP_LOGI("Avionics-Init", "Initializing task registry...");
    TaskRegistry::getInstance().registerTask(std::make_shared<TestTask>());

    TaskRegistry::getInstance().initTasks();

    vTaskDelay(pdMS_TO_TICKS(1000));

    // Testing Redundant Storage
    SpiffsStorage spiffsStorage;
    SDCardStorage sdCardStorage;
    RedundantStorageManager storageManager(&spiffsStorage, &sdCardStorage);

    storageManager.begin();

    if(!storageManager.appendToLog("/test.log", "Hello World Redundancy!")) {
        ESP_LOGE("Avionics-Init", "Failed to append to log");
    }

    auto raw = storageManager.readFromFile("/test.log");
    if (raw.empty()) {
        ESP_LOGE("Avionics-Init", "Failed to read from file");
    } else {
        ESP_LOGI("Avionics-Init", "Read from file: %s", raw.c_str());
    }
}
