// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef REDUNDANTSTORAGEMANAGER_H
#define REDUNDANTSTORAGEMANAGER_H
#include <Storage.h>


class RedundantStorageManager {
    public:¨
        static RedundantStorageManager& getInstance();
        //RedundantStorageManager(Storage* primary, Storage* secondary) : primaryStorage(primary), secondaryStorage(secondary), primaryReady(false), secondaryReady(false) {}

        void init(Storage* primary, Storage* secondary);
        //void init(Storage* primary, Storage* secondary, bool primaryReady, bool secondaryReady
        void begin();
        void end();
        bool appendToLog(const string& filename, const string& logEntry);
        bool appendCsvData(const string& filename, const string& csvLine);
        string readFromFile(const string& filename) const;
        [[nodiscard]] bool isPrimaryReady() const { return primaryReady; }
        [[nodiscard]] bool isSecondaryReady() const { return secondaryReady; }

    private:
        static const char* TAG;

        RedundantStorageManager() = default;
        RedundantStorageManager(const RedundantStorageManager&) = delete;
        RedundantStorageManager& operator=(const RedundantStorageManager&) = delete;

        Storage* primaryStorage;
        Storage* secondaryStorage;
        bool primaryReady = false;
        bool secondaryReady = false;
};



#endif //REDUNDANTSTORAGEMANAGER_H
