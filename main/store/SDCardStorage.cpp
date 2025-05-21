// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "SDCardStorage.h"
#include "../config/PinConfiguration.h"

#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <sdmmc_cmd.h>

const char* SDCardStorage::TAG = "SDCardStorage";

sdmmc_card_t* card = nullptr;
const char* mount_point = "/sdcard";

bool SDCardStorage::begin() {
    esp_err_t ret;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ESP_LOGI(TAG, "Initializing SPI bus %d...", SDSPI_HOST_ID);
    ret = spi_bus_initialize(SDSPI_HOST_ID, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus (%s)", esp_err_to_name(ret));
        return false;
    }

    sdmmc_host_t host_config = SDSPI_HOST_DEFAULT();
    host_config.slot = SDSPI_HOST_ID;
    host_config.max_freq_khz = 5000;

    sdspi_device_config_t dev_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    dev_config.host_id = host_config.slot;
    dev_config.gpio_cs = PIN_NUM_CS;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    ESP_LOGI(TAG, "Mounting SD card at %s...", mount_point);
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host_config, &dev_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. Consider setting 'format_if_mount_failed = true' if this is a new card or needs reformatting.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Check wiring, pull-ups, and power.", esp_err_to_name(ret));
        }
        spi_bus_free(SDSPI_HOST_ID); // Free SPI bus on failure
        return false;
    }

    ESP_LOGI(TAG, "SD card mounted successfully to %s.", mount_point);

    if (card) {
        sdmmc_card_print_info(stdout, card);
    }

    return true;
}

bool SDCardStorage::end() {
    if (card == nullptr) {
        ESP_LOGW(TAG, "SD card not mounted, nothing to unmount.");
        return true;
    }

    esp_err_t ret = esp_vfs_fat_sdcard_unmount(mount_point, card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to unmount SD card (%s)", esp_err_to_name(ret));
        return false;
    }
    ESP_LOGI(TAG, "SD card unmounted.");

    // Deinitialize SPI bus
    ret = spi_bus_free(SDSPI_HOST_ID);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to free SPI bus (%s)", esp_err_to_name(ret));
        return false;
    }
    card = nullptr; // Clear card pointer
    return true;
}

bool SDCardStorage::writeFile(const std::string& path, const std::string& data) {
    if (card == nullptr) {
        ESP_LOGE(TAG, "SD card not mounted. Call begin() first.");
        return false;
    }

    std::string full_path = std::string(mount_point) + path;
    FILE* f = fopen(full_path.c_str(), "w");
    if (f == nullptr) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", full_path.c_str());
        return false;
    }
    fprintf(f, "%s", data.c_str());
    fclose(f);
    ESP_LOGI(TAG, "File written: %s", full_path.c_str());
    return true;
}

std::string SDCardStorage::readFile(const std::string& path) {
    if (card == nullptr) {
        ESP_LOGE(TAG, "SD card not mounted. Call begin() first.");
        return "";
    }

    std::string full_path = std::string(mount_point) + path;
    FILE* f = fopen(full_path.c_str(), "r");
    if (f == nullptr) {
        ESP_LOGE(TAG, "Failed to open file for reading: %s", full_path.c_str());
        return "";
    }

    char line[128]; // Or allocate dynamically based on file size
    std::string content = "";
    while (fgets(line, sizeof(line), f) != nullptr) {
        content += line;
    }
    fclose(f);
    ESP_LOGI(TAG, "File read: %s", full_path.c_str());
    return content;
}

bool SDCardStorage::appendFile(const std::string& path, const std::string& data) {
    if (card == nullptr) {
        ESP_LOGE(TAG, "SD card not mounted. Call begin() first.");
        return false;
    }

    std::string full_path = std::string(mount_point) + path;
    FILE* f = fopen(full_path.c_str(), "a"); // "a" for append mode
    if (f == nullptr) {
        ESP_LOGE(TAG, "Failed to open file for appending: %s", full_path.c_str());
        return false;
    }
    fprintf(f, "%s", data.c_str());
    fclose(f);
    ESP_LOGI(TAG, "File appended: %s", full_path.c_str());
    return true;
}

bool SDCardStorage::deleteFile(const std::string& path) {
    if (card == nullptr) {
        ESP_LOGE(TAG, "SD card not mounted. Call begin() first.");
        return false;
    }

    std::string full_path = std::string(mount_point) + path;
    if (remove(full_path.c_str()) != 0) {
        ESP_LOGE(TAG, "Failed to delete file: %s", full_path.c_str());
        return false;
    }
    ESP_LOGI(TAG, "File deleted: %s", full_path.c_str());
    return true;
}

bool SDCardStorage::fileExists(const std::string& path) {
    if (card == nullptr) {
        ESP_LOGE(TAG, "SD card not mounted. Call begin() first.");
        return false;
    }

    std::string full_path = std::string(mount_point) + path;
    FILE* f = fopen(full_path.c_str(), "r");
    if (f != nullptr) {
        fclose(f);
        ESP_LOGI(TAG, "File exists: %s", full_path.c_str());
        return true;
    }
    return false;
}

size_t SDCardStorage::getTotalSpace() {
    if (card == nullptr) {
        ESP_LOGW(TAG, "SD card not mounted. Cannot get total space.");
        return 0;
    }
    // Total capacity in bytes
    return (size_t)card->csd.capacity * card->csd.sector_size;
}

size_t SDCardStorage::getUsedSpace() {
    /*if (card == nullptr) {
        ESP_LOGW(TAG, "SD card not mounted. Cannot get used space.");
        return 0;
    }

    FATFS* fs;
    DWORD fre_clust, tot_clust;
    esp_err_t ret = esp_vfs_fat_get_sdcard_info(mount_point, card, &fs);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SD card info for used space (%s)", esp_err_to_name(ret));
        return 0;
    }

    f_getfree("/", &fre_clust, &fs);
    tot_clust = (fs->n_fatent - 2);
    // Used space in bytes
    return (size_t)(tot_clust - fre_clust) * fs->csize * 512; // fs->csize is cluster size in sectors, 512 is sector size*/
    return 0;
}