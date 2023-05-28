#include "../include/strategy_1.hpp"

namespace uniorb {

strategy_1::strategy_1() : strategy("strategy_1") {}

size_t strategy_1::get_observed_satellite(size_t StationID, propagator_data & Data) const {
    // Рассматривает только спутники в зоне видимости, которые не ведут скачку на другие станции.
    double MaxStorage = 0;
    double UsedStorage = 0;
    size_t SatID = 0; 

    // Возвращаем спутник, у которого максимальное заполнение бортовой памяти + который не передает данные на другую станцию.
    for (auto SatelliteID : Data.stations.at(StationID).visible_satellites) {
        auto & Satellite = Data.satellites.at(SatelliteID);
        // Проверяем, что спутник не передают данные на другую станцию
        if (Satellite.station_id == 0 || Satellite.station_id == StationID) {
            UsedStorage = Satellite.get_used_storage();
            if (UsedStorage > MaxStorage) {
                MaxStorage = UsedStorage;
                SatID = SatelliteID;
            }
        }
    }
    return SatID;
}

}