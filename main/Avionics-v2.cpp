#include <esp_log.h>
#include <FlightState.h>
#include <NVSStore.h>
#include <SDCardStorage.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static FlightState* flightState = nullptr;

extern "C" void app_main(void)
{
    // Init
    if (NVSStore::initNVSFlash() != ESP_OK) {
        ESP_LOGE("Avionics-Init", "Failed to initialize NVS flash. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }
    flightState = new FlightState();

    vTaskDelay(pdMS_TO_TICKS(1000));

    // Testing the SDCard reader
    SDCardStorage sdCardStorage;
    if (!sdCardStorage.begin()) {
        ESP_LOGE("SDCardStorage", "Failed to initialize SD card");
        return;
    }

    sdCardStorage.writeFile("/test.txt", "Hello, SD Card!");
    std::string data = sdCardStorage.readFile("/test.txt");
    if (!data.empty()) {
        ESP_LOGI("SDCardStorage", "Read from SD card: %s", data.c_str());
    } else {
        ESP_LOGE("SDCardStorage", "Failed to read from SD card");
    }

    sdCardStorage.end();
}
