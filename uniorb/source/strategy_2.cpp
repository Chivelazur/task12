#include "../include/strategy_2.hpp"

namespace uniorb {

strategy_2::strategy_2() : strategy("strategy_2") {}

size_t strategy_2::get_observed_satellite(size_t StationID, propagator_data & Data) const {
    double UsedStorage = 0;
    double MaxStorage = 0;
    double MaxRate = 0;
    double Rate = 0;
    
    size_t SatID = 0; 
    // Возвращаем спутник, у которого самая быстрая передача данных + который не передает данные на другую станцию.
    for (auto SatelliteID : Data.stations.at(StationID).visible_satellites) {
        auto & Satellite = Data.satellites.at(SatelliteID);
        // Проверяем, что спутник не передают данные на другую станцию
        if (Satellite.station_id == 0 || Satellite.station_id == StationID) {
            UsedStorage = Satellite.get_used_storage();
            if (UsedStorage > 0) {
                Rate = Satellite.parameters.output_rate;
                if (Rate > MaxRate) {
                    MaxRate = Rate;
                    SatID = SatelliteID;
                }
            }
        }
    }
    return SatID;
}

}