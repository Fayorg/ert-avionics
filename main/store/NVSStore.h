//
// Created by Fayorg on 21/05/2025.
//

#ifndef NVSSTORE_H
#define NVSSTORE_H

#include <cstdint>
#include <esp_err.h>
#include <nvs.h>
#include <string>


class NVSStore {
    public:
        // @param ns_name The name of the NVS namespace to use.
        NVSStore(const std::string& ns_name);

        ~NVSStore();

        // Initializes the NVS flash partition. Should be called once at startup.
        // Returns ESP_OK on success, an error code otherwise.
        static esp_err_t initNVSFlash();

        // Sets an integer value in NVS.
        // @param key The key to store the value under.
        // @param value The integer value to store.
        // Returns ESP_OK on success, an error code otherwise.
        esp_err_t setInt(const std::string& key, int32_t value) const;

        // Gets an integer value from NVS.
        // @param key The key of the value to retrieve.
        // @param defaultValue The value to return if the key is not found.
        // Returns the retrieved value, or defaultValue if not found or on error.
        int32_t getInt(const std::string &key, int32_t defaultValue = 0) const;

        // Sets a string value in NVS.
        // @param key The key to store the value under.
        // @param value The string value to store.
        // Returns ESP_OK on success, an error code otherwise.
        esp_err_t setString(const std::string& key, const std::string& value) const;

        // Gets a string value from NVS.
        // @param key The key of the value to retrieve.
        // @param defaultValue The value to return if the key is not found.
        // Returns the retrieved string, or defaultValue if not found or on error.
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;

        // Commits all pending changes to flash.
        // This *must* be called after set operations to ensure data persists.
        // Returns ESP_OK on success, an error code otherwise.
        esp_err_t commit() const;

        // Erases a specific key-value pair from the current namespace.
        // @param key The key to erase.
        // Returns ESP_OK on success, or an error code if the key doesn't exist or on error.
        esp_err_t eraseKey(const std::string& key) const;

        // Erases all key-value pairs in the current namespace.
        // Returns ESP_OK on success, or an error code otherwise.
        esp_err_t eraseAll() const;

    private:
        std::string _namespaceName;
        nvs_handle_t _nvsHandle;
        bool _isInitialized; // To track if the NVS handle was successfully opened

        static const char* TAG; // Tag for ESP_LOG messages
};

#endif //NVSSTORE_H