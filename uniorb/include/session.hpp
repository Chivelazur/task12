#pragma once

#include <string>

namespace uniorb {

struct session {
public:
    // ID спутника
    size_t satellite_id;

    // ID станции
    size_t station_id;

    // MJD начала сессии.
    double start_date;

    // MJD конца сессии.
    double end_date;

    // Объем переданных данных, GB
    double passed_data = 0;

    session(size_t SatelliteID, size_t StationID, double StartDate);
};

}