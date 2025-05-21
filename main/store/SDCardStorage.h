// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef SDCARDSTORAGE_H
#define SDCARDSTORAGE_H

#include "Storage.h"

class SDCardStorage : public Storage {
    public:
        SDCardStorage() = default;
        ~SDCardStorage() = default;

        bool begin() override;
        bool end() override;
        bool writeFile(const string& path, const string& data) override;

        string readFile(const string& path) override;
        bool appendFile(const string& path, const string& data) override;
        bool deleteFile(const string& path) override;
        bool fileExists(const string& path) override;
        string getName() override { return "SD"; }
        size_t getTotalSpace() override;
        size_t getUsedSpace() override;

    private:
        static const char* TAG;
};

#endif //SDCARDSTORAGE_H
