//
// Created by Fayorg on 21/05/2025.
//

#ifndef FLIGHTSTATE_H
#define FLIGHTSTATE_H

#include <NVSStore.h>

class FlightState {
    public:
        enum State {
            INIT = 0,
            READY = 1,
            IN_FLIGHT = 2,
            GLIDING = 3,
            LANDED = 4
        };
        static FlightState& getInstance();

        static void initFlightState();

        bool setState(State new_state);
        State getState() const;

        // Refresh state from memory in case of unexpected shutdown
        bool refreshState();

    private:
        FlightState() = default;
        FlightState(const FlightState&) = delete;
        FlightState& operator=(const FlightState&) = delete;

        State current_state;
        NVSStore store;

        static const char* TAG;
};

#endif //FLIGHTSTATE_H