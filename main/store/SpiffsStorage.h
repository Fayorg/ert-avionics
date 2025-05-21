//
// Created by Fayorg on 21/05/2025.
//

#ifndef SPIFFSSTORAGE_H
#define SPIFFSSTORAGE_H
#include "Storage.h"
#include <string>

using namespace std;

class SpiffsStorage : public Storage {
    public:
        SpiffsStorage() = default;
        ~SpiffsStorage() = default;

        bool begin() override;
        bool end() override;
        bool writeFile(const string& path, const string& data) override;

        string readFile(const string& path) override;
        bool appendFile(const string& path, const string& data) override;
        bool deleteFile(const string& path) override;
        bool fileExists(const string& path) override;
        string getName() override { return "SPIFFS"; }
        size_t getTotalSpace() override;
        size_t getUsedSpace() override;

    private:
        static const char* TAG;
};



#endif //SPIFFSSTORAGE_H
