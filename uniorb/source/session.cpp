#include "../include/session.hpp"

namespace uniorb {

session::session(size_t SatelliteID, double StartDate) :
    satellite_id(SatelliteID),
    start_date(StartDate),
    end_date(-1),
    passed_data(0) {}

}