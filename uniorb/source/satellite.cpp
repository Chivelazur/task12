#include "../include/satellite.hpp"
#include <algorithm>

namespace uniorb {

size_t satellite::_id = 1;

satellite::satellite(const std::string & Name, const satellite_parameters & Parameters) : 
    id(_id++), name(Name), parameters(Parameters), is_russia(false), station_id(0), _used_storage(0), _transferred_data(0), _camera_data(0) {}

satellite::satellite(const satellite & Satellite) : 
    id(Satellite.id), 
    name(Satellite.name), 
    parameters(Satellite.parameters), 
    is_russia(Satellite.is_russia),
    station_id(Satellite.station_id),
    _used_storage(Satellite._used_storage),
    _camera_data(Satellite._camera_data),
    _transferred_data(Satellite._transferred_data) {}

satellite::satellite(satellite && Satellite) : 
    id(Satellite.id), 
    name(std::move(Satellite.name)), 
    parameters(std::move(Satellite.parameters)),
    is_russia(Satellite.is_russia),
    station_id(Satellite.station_id),
    _used_storage(Satellite._used_storage),
    _camera_data(Satellite._camera_data),
    _transferred_data(Satellite._transferred_data) {}

satellite & satellite::operator=(const satellite & Satellite) {
    id = Satellite.id;
    name = Satellite.name;
    parameters = Satellite.parameters;
    is_russia = Satellite.is_russia;
    station_id = Satellite.station_id;
    _used_storage = Satellite._used_storage;
    _camera_data = Satellite._camera_data;
    _transferred_data = Satellite._transferred_data;
    return *this;
}

satellite & satellite::operator=(satellite && Satellite) {
    id = Satellite.id;
    name = std::move(Satellite.name);
    parameters = std::move(Satellite.parameters);
    is_russia = Satellite.is_russia;
    station_id = Satellite.station_id;
    _used_storage = Satellite._used_storage;
    _camera_data = Satellite._camera_data;
    _transferred_data = Satellite._transferred_data;
    return *this;
}

double satellite::increase_storage(double Value) {
    double Added = std::min(Value, parameters.storage - _used_storage);
    _used_storage += Added;
    _camera_data += Added;
    camera.back().passed_data += Added;
    return Added;
}

double satellite::decrease_storage(double Value) {
    _used_storage = _used_storage - Value;
    if (_used_storage < 0) {
        _used_storage = 0;
        _transferred_data += _used_storage;
        return _used_storage;
    }
    _transferred_data += Value;
    return Value;
}

double satellite::get_used_storage() const {
    return _used_storage;
}

bool satellite::is_overloaded() const {
    if ( std::abs(_used_storage - parameters.storage) < 1E-10) return true;
    return false;
}
 

double satellite::get_transferred_data() const {
    return _transferred_data;
}

double satellite::get_camera_data() const {
    return _camera_data;
}

bool satellite::is_loading() const {
    return (station_id != 0);
}

bool satellite::is_camera() const {
    return (station_id == 0 && is_russia && !is_overloaded());
}

void satellite::finish_session(double EndMjd) {
    if (station_id != 0) {
        sessions.back().end_date = EndMjd;
        station_id = 0;
    }
}

void satellite::start_session(double StartMjd, size_t StationID) {
    if (station_id == 0) {
        sessions.push_back(session(_id, StationID, StartMjd));
        station_id = StationID;
    }
}

void satellite::start_camera(double StartMjd) {
    camera.push_back(session(_id, 0, StartMjd));
}

void satellite::finish_camera(double EndMjd) {
    if (camera.size() != 0) {
        camera.back().end_date = EndMjd;
    }
}

}