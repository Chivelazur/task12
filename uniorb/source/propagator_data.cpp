#include "../include/propagator_data.hpp"

namespace uniorb {

propagator_data::propagator_data(const std::vector<satellite> & SatelliteVector, const std::vector<station> & StationVector) {
    satellites = std::unordered_map<size_t, satellite> ();
    stations = std::unordered_map<size_t, station> ();
    metrics = std::vector<std::pair<event, metrica>> ();

    for (auto const & Station : StationVector) {
        stations.insert_or_assign(Station.id, Station);
    }
    for (auto const & Satellite : SatelliteVector) {
        satellites.insert_or_assign(Satellite.id, Satellite);
    }
}

}