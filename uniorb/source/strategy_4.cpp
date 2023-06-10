#include "../include/strategy_4.hpp"

namespace uniorb {

strategy_4::strategy_4() : strategy("strategy_4") {}

size_t strategy_4::get_observed_satellite(const settings & Settings, size_t StationID, propagator_data & Data) const {
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

            // Если это Киноспутник, который не ведет съемку - берем его, если бортовая память заполнена более чем на 19%.
            if (Satellite.parameters.output_rate == Settings.kinosat_parameters.output_rate && !Satellite.is_russia) {
                if (UsedStorage > Satellite.parameters.storage * 0.19)
                return SatelliteID;
            }

            // Иначе смотрим максимальные заполнения по всем спутникам.
            if (UsedStorage > MaxStorage) {
                MaxStorage = UsedStorage;
                SatID = SatelliteID;
            }
        }
    }
    return SatID;
}

}