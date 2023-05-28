#pragma once

#include <string>
#include "event_type.hpp"

namespace uniorb {

struct event {
public:
     // MJD, модифицированная юлианская дата
    double epoch;
    
    // Тип события
    event_type type;

    // ID станции
    size_t station_id;

    // ID спутника
    size_t satellite_id;

    static bool compare_events(const event & A, const event & B);
    
};

}