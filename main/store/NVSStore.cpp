//
// Created by Fayorg on 21/05/2025.
//

#include "NVSStore.h"

#include <esp_log.h>
#include <nvs_flash.h>
#include <cstring> // For strlen

const char* NVSStore::TAG = "NVSStore";

NVSStore::NVSStore(const std::string& ns_name) : _namespaceName(ns_name), _nvsHandle(0), _isInitialized(false) {
    esp_err_t err = nvs_open(_namespaceName.c_str(), NVS_READWRITE, &_nvsHandle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace '%s': %s", _namespaceName.c_str(), esp_err_to_name(err));
        _isInitialized = false;
    } else {
        ESP_LOGI(TAG, "NVS namespace '%s' opened successfully.", _namespaceName.c_str());
        _isInitialized = true;
    }
}

NVSStore::~NVSStore() {
    if (_isInitialized) {
        nvs_close(_nvsHandle);
        ESP_LOGI(TAG, "NVS handle for namespace '%s' closed.", _namespaceName.c_str());
    }
}

esp_err_t NVSStore::initNVSFlash() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition is not formatted or contains new version. Erasing and re-initializing...");
        ESP_ERROR_CHECK(nvs_flash_erase()); // Erase and try again
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS flash: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "NVS flash initialized successfully.");
    }
    return ret;
}

esp_err_t NVSStore::setInt(const std::string& key, int32_t value) const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot set key '%s'.", _namespaceName.c_str(), key.c_str());
        return ESP_FAIL;
    }
    if (key.length() > 15) { // NVS key limit
        ESP_LOGE(TAG, "Key '%s' is too long (max 15 characters).", key.c_str());
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = nvs_set_i32(_nvsHandle, key.c_str(), value);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set int '%s' in NVS: %s", key.c_str(), esp_err_to_name(err));
    } else {
        ESP_LOGD(TAG, "Set int '%s' to %" PRId32 ".", key.c_str(), value);
    }
    return err;
}

int32_t NVSStore::getInt(const std::string& key, int32_t defaultValue) const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot get key '%s'. Returning default.", _namespaceName.c_str(), key.c_str());
        return defaultValue;
    }

    int32_t value;
    esp_err_t err = nvs_get_i32(_nvsHandle, key.c_str(), &value);
    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Got int '%s': %" PRId32 ".", key.c_str(), value);
        return value;
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "Key '%s' not found in NVS. Returning default value %" PRId32 ".", key.c_str(), defaultValue);
        return defaultValue;
    } else {
        ESP_LOGE(TAG, "Failed to get int '%s' from NVS: %s. Returning default.", key.c_str(), esp_err_to_name(err));
        return defaultValue;
    }
}

esp_err_t NVSStore::setString(const std::string& key, const std::string& value) const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot set key '%s'.", _namespaceName.c_str(), key.c_str());
        return ESP_FAIL;
    }
    if (key.length() > 15) { // NVS key limit
        ESP_LOGE(TAG, "Key '%s' is too long (max 15 characters).", key.c_str());
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = nvs_set_str(_nvsHandle, key.c_str(), value.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set string '%s' in NVS: %s", key.c_str(), esp_err_to_name(err));
    } else {
        ESP_LOGD(TAG, "Set string '%s' to '%s'.", key.c_str(), value.c_str());
    }
    return err;
}

std::string NVSStore::getString(const std::string& key, const std::string& defaultValue) const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot get key '%s'. Returning default.", _namespaceName.c_str(), key.c_str());
        return defaultValue;
    }

    size_t required_size;
    esp_err_t err = nvs_get_str(_nvsHandle, key.c_str(), NULL, &required_size); // Get required size
    if (err == ESP_OK) {
        std::string value(required_size, '\0'); // Create string with enough capacity
        err = nvs_get_str(_nvsHandle, key.c_str(), &value[0], &required_size); // Read data
        if (err == ESP_OK) {
            value.pop_back(); // Remove the null terminator added by NVS
            ESP_LOGD(TAG, "Got string '%s': '%s'.", key.c_str(), value.c_str());
            return value;
        } else {
            ESP_LOGE(TAG, "Failed to get string data for '%s' from NVS: %s. Returning default.", key.c_str(), esp_err_to_name(err));
            return defaultValue;
        }
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "Key '%s' not found in NVS. Returning default value '%s'.", key.c_str(), defaultValue.c_str());
        return defaultValue;
    } else {
        ESP_LOGE(TAG, "Failed to get string size for '%s' from NVS: %s. Returning default.", key.c_str(), esp_err_to_name(err));
        return defaultValue;
    }
}

esp_err_t NVSStore::commit() const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot commit.", _namespaceName.c_str());
        return ESP_FAIL;
    }
    esp_err_t err = nvs_commit(_nvsHandle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit NVS changes for namespace '%s': %s", _namespaceName.c_str(), esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "NVS changes for namespace '%s' committed.", _namespaceName.c_str());
    }
    return err;
}

esp_err_t NVSStore::eraseKey(const std::string& key) const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot erase key '%s'.", _namespaceName.c_str(), key.c_str());
        return ESP_FAIL;
    }
    esp_err_t err = nvs_erase_key(_nvsHandle, key.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase key '%s' from NVS: %s", key.c_str(), esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Key '%s' erased from NVS.", key.c_str());
    }
    return err;
}

esp_err_t NVSStore::eraseAll() const {
    if (!_isInitialized) {
        ESP_LOGE(TAG, "NVS not initialized for namespace '%s'. Cannot erase all.", _namespaceName.c_str());
        return ESP_FAIL;
    }
    esp_err_t err = nvs_erase_all(_nvsHandle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase all keys in namespace '%s': %s", _namespaceName.c_str(), esp_err_to_name(err));
    } else {
        ESP_LOGW(TAG, "All keys in namespace '%s' erased.", _namespaceName.c_str());
    }
    return err;
}
