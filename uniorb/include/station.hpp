#pragma once

#include <string>
#include <unordered_set>

#include "session.hpp"

namespace uniorb {

struct station {
public:
    std::string name;
    size_t id;

    // ID спутника, с которого скачивает данные (0 если не скачивает).
    size_t satellite_id = 0;

    // Список ID спутников в зоне видимости станции.
    std::unordered_set<size_t> visible_satellites = std::unordered_set<size_t>();

    // Сеансы передачи данных со спутников.
    std::vector<session> sessions = std::vector<session>();

    station(const std::string & Name);
    station(const station & Station);
    station(station && Station);
    station & operator=(const station & Station);
    station & operator=(station && Station);

    // Скачивает ли станция данные со спутника
    bool is_loading() const;

    // Завершает текущий сеанс: ставит в 0 satellite_id, устанавливает конец сессии.
    void finish_session(double EndMjd);

    // Стартует новый сеанс, если нет текущего.
    void start_session(double StartMjd, size_t SatelliteID);
    
private:
    // Автоматически присваемый ID при создании объекта. Начинается с 1.
    static size_t _id;
};

}