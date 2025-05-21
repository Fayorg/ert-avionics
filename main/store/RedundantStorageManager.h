// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef REDUNDANTSTORAGEMANAGER_H
#define REDUNDANTSTORAGEMANAGER_H
#include <Storage.h>


class RedundantStorageManager {
    public:
        RedundantStorageManager(Storage* primary, Storage* secondary) : primaryStorage(primary), secondaryStorage(secondary), primaryReady(false), secondaryReady(false) {}

        void begin();
        void end();
        bool appendToLog(const string& filename, const string& logEntry);
        bool appendCsvData(const string& filename, const string& csvLine);
        string readFromFile(const string& filename) const;
        [[nodiscard]] bool isPrimaryReady() const { return primaryReady; }
        [[nodiscard]] bool isSecondaryReady() const { return secondaryReady; }

    private:
        static const char* TAG;
        Storage* primaryStorage;
        Storage* secondaryStorage;
        bool primaryReady;
        bool secondaryReady;
};



#endif //REDUNDANTSTORAGEMANAGER_H
