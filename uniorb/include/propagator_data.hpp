#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "event.hpp"
#include "station.hpp"
#include "satellite.hpp"
#include "session.hpp"
#include "metrica.hpp"

namespace uniorb {

struct propagator_data {
public:
    propagator_data(const std::vector<satellite> & SatelliteVector, const std::vector<station> & StationVector);

    // Словарь <ID спутника, Спутник>
    std::unordered_map<size_t, satellite> satellites;

    // Словарь <ID станции, Станция>
    std::unordered_map<size_t, station> stations;

    // Словарь <ID события, метрики> ключевыъ метрик.
    std::vector<std::pair<event, metrica>> metrics; 
};

}