#include "../include/session.hpp"

namespace uniorb {

session::session(size_t SatelliteID, size_t StationID, double StartDate) :
    satellite_id(SatelliteID),
    station_id(StationID),
    start_date(StartDate),
    end_date(-1),
    passed_data(0) {}

}