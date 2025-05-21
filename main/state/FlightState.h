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

        FlightState();

        bool setState(State new_state);
        [[nodiscard]] State getState() const;

        // Refresh state from memory in case of unexpected shutdown
        bool refreshState();

    private:
        State current_state;
        NVSStore store;

        static const char* TAG;
};

#endif //FLIGHTSTATE_H