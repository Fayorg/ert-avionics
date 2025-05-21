//
// Created by Fayorg on 21/05/2025.
//

#ifndef STORAGE_H
#define STORAGE_H
#include <string>

using namespace std;

class Storage {
    public:
        virtual bool begin() = 0;
        virtual bool end() = 0;
        virtual bool writeFile(const string& path, const string& data) = 0;
        virtual string readFile(const string& path) = 0;
        virtual bool appendFile(const string& path, const string& data) = 0;
        virtual bool deleteFile(const string& path) = 0;
        virtual bool fileExists(const string& path) = 0;
        virtual string getName() = 0;
        virtual size_t getTotalSpace() = 0;
        virtual size_t getUsedSpace() = 0;
};



#endif //STORAGE_H
