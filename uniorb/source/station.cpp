#include "../include/station.hpp"

namespace uniorb {

size_t station::_id = 1;

station::station (const std::string & Name) : 
    id(_id++), name(Name), satellite_id(0), sessions(), visible_satellites() {}

station::station(const station & Station) : 
    id(Station.id), 
    name(Station.name),
    satellite_id(Station.satellite_id),
    sessions(Station.sessions),
    visible_satellites(Station.visible_satellites) {}

station::station(station && Station) : 
    id(Station.id), 
    name(std::move(Station.name)),
    satellite_id(Station.satellite_id),
    sessions(std::move(Station.sessions)),
    visible_satellites(std::move(Station.visible_satellites)) {}

station & station::operator=(const station & Station) {
    id = Station.id;
    name = Station.name;
    satellite_id = Station.satellite_id;
    sessions = Station.sessions;
    visible_satellites = Station.visible_satellites;
    return *this;
}

station & station::operator=(station && Station) {
    id = Station.id;
    name = std::move(Station.name);
    satellite_id = Station.satellite_id;
    sessions = std::move(Station.sessions);
    visible_satellites = std::move(Station.visible_satellites);
    return *this;
}

bool station::is_loading() const {
    return (satellite_id != 0);
}

void station::finish_session(double EndMjd) {
    if (satellite_id != 0) {
        sessions.back().end_date = EndMjd;
        satellite_id = 0;
    }
}

void station::start_session(double StartMjd, size_t SatelliteID) {
    if (satellite_id == 0) {
        sessions.push_back(session(SatelliteID, _id, StartMjd));
        satellite_id = SatelliteID;
    }
}

}