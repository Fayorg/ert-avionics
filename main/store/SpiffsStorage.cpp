//
// Created by Fayorg on 21/05/2025.
//

#include "SpiffsStorage.h"
#include <esp_log.h>
#include <esp_spiffs.h>

const char* SpiffsStorage::TAG = "SpiffsStorage";
const char* LABEL = "spiffs";

bool SpiffsStorage::begin() {
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = LABEL,
        .max_files = 5,
        .format_if_mount_failed = true
      };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return false;
    }

    ESP_LOGI(TAG, "SPIFFS initialized.");
    size_t total = 0, used = 0;
    esp_spiffs_info(conf.partition_label, &total, &used);
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return true;
}

bool SpiffsStorage::end() {
    ESP_LOGI(TAG, "Unmounting SPIFFS...");
    esp_vfs_spiffs_unregister(LABEL);
    ESP_LOGI(TAG, "SPIFFS unmounted.");
    return true;
}

bool SpiffsStorage::writeFile(const std::string& path, const std::string& data) {
    std::string full_path = "/spiffs" + path;
    ESP_LOGI(TAG, "Writing file: %s", full_path.c_str());

    FILE* f = fopen(full_path.c_str(), "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", full_path.c_str());
        return false;
    }

    if (fprintf(f, "%s", data.c_str()) < 0) {
        ESP_LOGE(TAG, "Failed to write to file: %s", full_path.c_str());
        fclose(f);
        return false;
    }
    fclose(f);
    ESP_LOGI(TAG, "File written successfully: %s", full_path.c_str());
    return true;
}

std::string SpiffsStorage::readFile(const std::string& path) {
    std::string full_path = "/spiffs" + path;
    ESP_LOGI(TAG, "Reading file: %s", full_path.c_str());

    FILE* f = fopen(full_path.c_str(), "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading: %s", full_path.c_str());
        return ""; // Return empty string on failure
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);

    ESP_LOGI(TAG, "File size: %ld bytes", fsize);
    fseek(f, 0, SEEK_SET);

    std::string data;
    data.reserve(fsize); // Pre-allocate memory

    int c;
    while ((c = fgetc(f)) != EOF) {
        data += static_cast<char>(c);
    }

    fclose(f);
    ESP_LOGI(TAG, "File read successfully: %s", full_path.c_str());
    return data;
}

bool SpiffsStorage::appendFile(const std::string& path, const std::string& data) {
    std::string full_path = "/spiffs" + path;
    ESP_LOGI(TAG, "Appending to file: %s", full_path.c_str());

    FILE* f = fopen(full_path.c_str(), "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for appending: %s", full_path.c_str());
        return false;
    }

    if (fprintf(f, "%s", data.c_str()) < 0) {
        ESP_LOGE(TAG, "Failed to append to file: %s", full_path.c_str());
        fclose(f);
        return false;
    }
    fclose(f);
    ESP_LOGI(TAG, "Data appended successfully to: %s", full_path.c_str());
    return true;
}

bool SpiffsStorage::deleteFile(const std::string& path) {
    std::string full_path = "/spiffs" + path;
    ESP_LOGI(TAG, "Deleting file: %s", full_path.c_str());

    if (remove(full_path.c_str()) != 0) {
        ESP_LOGE(TAG, "Failed to delete file: %s", full_path.c_str());
        return false;
    }
    ESP_LOGI(TAG, "File deleted successfully: %s", full_path.c_str());
    return true;
}

bool SpiffsStorage::fileExists(const std::string& path) {
    std::string full_path = "/spiffs" + path;
    ESP_LOGI(TAG, "Checking if file exists: %s", full_path.c_str());

    FILE* f = fopen(full_path.c_str(), "r");
    if (f != NULL) {
        fclose(f);
        ESP_LOGI(TAG, "File exists: %s", full_path.c_str());
        return true;
    }
    ESP_LOGI(TAG, "File does not exist: %s", full_path.c_str());
    return false;
}

size_t SpiffsStorage::getTotalSpace() {
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS info: %s", esp_err_to_name(ret));
        return 0;
    }
    return total;
}

size_t SpiffsStorage::getUsedSpace() {
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS info: %s", esp_err_to_name(ret));
        return 0;
    }
    return used;
}