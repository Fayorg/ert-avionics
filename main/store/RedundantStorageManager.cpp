// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "RedundantStorageManager.h"

#include <esp_log.h>

const char* RedundantStorageManager::TAG = "RedundantStorageManager";

void RedundantStorageManager::begin() {
    ESP_LOGI(TAG, "Initializing storage...");
    primaryReady = primaryStorage->begin();
    if (primaryReady) {
        ESP_LOGI(TAG, "Primary storage initialized successfully (%s)", primaryStorage->getName().c_str());
    } else {
        ESP_LOGE(TAG, "Failed to initialize primary storage (%s)", primaryStorage->getName().c_str());
    }

    secondaryReady = secondaryStorage->begin();
    if (secondaryReady) {
        ESP_LOGI(TAG, "Secondary storage initialized successfully (%s)", secondaryStorage->getName().c_str());
    } else {
        ESP_LOGE(TAG, "Failed to initialize secondary storage (%s)", secondaryStorage->getName().c_str());
    }
}

void RedundantStorageManager::end() {
    ESP_LOGI(TAG, "Ending storage...");
    if (!primaryStorage->end()) {
        ESP_LOGE(TAG, "Failed to end primary storage (%s)", primaryStorage->getName().c_str());
    }
    if (!secondaryStorage->end()) {
        ESP_LOGE(TAG, "Failed to end secondary storage (%s)", secondaryStorage->getName().c_str());
    }
}


// TODO: Retry failed storage with exponential backoff
bool RedundantStorageManager::appendToLog(const string &filename, const string &logEntry) {
    bool successPrimary = false;
    bool successSecondary = false;

    if (primaryReady) {
        successPrimary = primaryStorage->appendFile(filename, logEntry);
        if (!successPrimary) {
            ESP_LOGW(TAG, "Primary (%s) failed to append to %s. Marking as not ready.", primaryStorage->getName().c_str(), filename.c_str());
            primaryReady = false; // Mark primary as failed
        }
    }

    if (secondaryReady && !successPrimary) { // Only try secondary if primary failed or not ready
        successSecondary = secondaryStorage->appendFile(filename, logEntry);
        if (!successSecondary) {
            ESP_LOGW(TAG, "Secondary (%s) failed to append to %s. Marking as not ready.", secondaryStorage->getName().c_str(), filename.c_str());
            secondaryReady = false; // Mark secondary as failed
        }
    } else if (secondaryReady && successPrimary) { // If primary succeeded, still try secondary for redundancy
        if (!secondaryStorage->appendFile(filename, logEntry)) {
            ESP_LOGW(TAG, "Secondary (%s) failed to append to %s, but Primary succeeded.", secondaryStorage->getName().c_str(), filename.c_str());
        }
    }

    if (successPrimary || successSecondary) {
        return true;
    } else {
        ESP_LOGE(TAG, "Both primary (%s) and secondary (%s) failed to append to %s.", primaryStorage->getName().c_str(), secondaryStorage->getName().c_str(), filename.c_str());
        return false;
    }
}

bool RedundantStorageManager::appendCsvData(const string& filename, const string& csvLine) {
    return appendToLog(filename, csvLine);
}

string RedundantStorageManager::readFromFile(const string& filename) const {
    string data = "";
    if (primaryReady) {
        data = primaryStorage->readFile(filename);
        if (data.length() > 0) {
            ESP_LOGI(TAG, "Read %d bytes from Primary (%s) file: %s", data.length(), primaryStorage->getName().c_str(), filename.c_str());
            return data;
        } else {
            ESP_LOGW(TAG, "Primary (%s) failed to read %s or file was empty. Marking as not ready.", primaryStorage->getName().c_str(), filename.c_str());
            // Don't mark primary as not ready for just an empty read, only if file open/read definitively failed
        }
    }

    if (secondaryReady) {
        data = secondaryStorage->readFile(filename);
        if (data.length() > 0) {
            ESP_LOGI(TAG, "Read %d bytes from Secondary (%s) file: %s", data.length(), secondaryStorage->getName().c_str(), filename.c_str());
            return data;
        } else {
            ESP_LOGW(TAG, "Secondary (%s) failed to read %s or file was empty. Marking as not ready.", secondaryStorage->getName().c_str(), filename.c_str());
        }
    }

    ESP_LOGE(TAG, "Both primary (%s) and secondary (%s) failed to read %s.", primaryStorage->getName().c_str(), secondaryStorage->getName().c_str(), filename.c_str());
    return "";
}

