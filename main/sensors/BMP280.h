//
// Created by maxwc on 02/04/2025.
//

#ifndef BMP280_H
#define BMP280_H

extern "C" {
    #include "../../components/bmp280/bmp280.h"
}


#include <Sensor.h>


class BMP280 : public Sensor {
public:
    //For all the overriden functions, check the SuperClass
    //Sensor. -> go to Sensor.h
    static BMP280& getInstance();

    bool init() override;

    bool read() override;
    void display() override; // purely for testing to the serial monitor
    void calibrate() override; // calulates the initial data for the first 100 readings

    static void deployement_sequence(bool deploy_main_para_parachute, bool deployed);
    static void testing();

    float get_temperature() const {return temperature;}
    float get_pressure() const {return pressure;}
    float get_altitude() const {return altitude;}
    float get_max_altitude() const {return max_altitude;}

    float get_starting_altitude() const {return starting_altitude;}
    float get_starting_pressure() const {return starting_pressure;}
    float get_starting_temperature() const {return starting_temperature;}

    bool get_deploy_main_para_parachute() const {return deploy_main_para_parachute;}
    bool get_max_altitude_reached() const {return max_altitude_reached;}

private:
    BMP280() = default;
    BMP280(const BMP280&) = delete;
    BMP280& operator=(const BMP280&) = delete;

    bmp280_t dev = {}; // the bmp280 parameters i2c params
    float temperature = .0; //BMP280 has two readings, pressure and temp, BME280 has humidity on top of that
    float pressure = .0;
    float altitude;

    float previous_temperature = 0.0f;
    float previous_pressure = 0.0f;
    float previous_altitude;

    float starting_altitude = 0.0f; // used to calculate the difference in altitude for the drogue
    float starting_pressure = 0.0f;
    float starting_temperature = 0.0f; // not really usefull but why not :)

    float max_altitude = 0.0f; // used to calculate the max altitude for the drogue
    bool max_altitude_reached = false;

    bool deploy_main_para_parachute = false;
    bool deployed = false;


    int64_t last_read = 0;
};





#endif //BMP280_H
