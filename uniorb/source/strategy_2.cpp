#include "../include/strategy_2.hpp"

namespace uniorb {

strategy_2::strategy_2() : strategy("strategy_2") {}

size_t strategy_2::get_observed_satellite(const settings & Settings, size_t StationID, propagator_data & Data) const {
    // Рассматривает только спутники в зоне видимости, которые не ведут скачку на другие станции.
    double UsedStorage = 0;
    double MaxStorage = 0;
    double MaxRate = 0;
    double Rate = 0;
    
    size_t SatID = 0; 
    // Возвращаем спутник, у которого самая быстрая передача данных + самый большой объем занятой бортовой памяти + который не передает данные на другую станцию.
    for (auto SatelliteID : Data.stations.at(StationID).visible_satellites) {
        auto & Satellite = Data.satellites.at(SatelliteID);
        // Проверяем, что спутник не передают данные на другую станцию
        if (Satellite.station_id == 0 || Satellite.station_id == StationID) {
            UsedStorage = Satellite.get_used_storage();
            Rate = Satellite.parameters.output_rate;
            // Если у спутника выше скорость передачи данных - он становится лидером. Далее сравниваем со спутниками, у которых не менее низкая скорость передачи данных
            if (UsedStorage > 0 && Rate > MaxRate) {
                MaxRate = Rate;
                MaxStorage = UsedStorage;
                SatID = SatelliteID;
            }
            // Если у спутника такая же скорость передачи данных, но больше занято памяти - он лидер.
            else if (Rate == MaxRate && UsedStorage > MaxStorage) {
                MaxStorage = UsedStorage;
                SatID = SatelliteID;
            }
        }
    }
    return SatID;
}

}