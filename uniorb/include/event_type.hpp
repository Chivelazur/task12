#pragma once

namespace uniorb {

// Перечисления типов событий
enum event_type : unsigned char{
    // Вход спутника S в зону видимости станции T.
    station_entrance = 0,

    // Выход спутника S из зоны видимости станции T.
    station_exit = 1,

    // Вход спутника S на территорию России.
    russia_entrance = 2,
 
    // Выход спутника S с территории России.
    russia_exit = 3,

    // Завершения скачивания всех данных со спутника
    download_finish = 4
};

}