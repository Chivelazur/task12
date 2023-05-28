#pragma once

#include <string>
#include <vector>

#include "event.hpp"
#include "station.hpp"
#include "satellite.hpp"
#include "settings.hpp"

namespace uniorb {

class importer {
public:
    importer() = delete;
    /*
    Вход:
        Settings - параметры загрузки.
        Events - выходной вектор событий, отсортированный по дате наступления событий по возрастанию.
        Satellites - выходной массив спутников
        Stations - выходной массив станций
    Выход:
        true - если загрузка успешна.
        false - если ошибочна.
    Важно: данные, который находятся в переданных Events, Satellites, Stations, очищаются.
    */
    static bool import_data(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites, std::vector<station> & Stations);

private:
    /* Аналогично import_data загружаются файлы пролета над территорией РФ, но не затирает данные на входе */
    static bool _import_russia_flights(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites);
    
    /* Аналогично import_data загружаются файлы видимостей спутников на станциях, но не затирает данные на входе */
    static bool _import_station_visibility(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites, std::vector<station> & Stations);

    /*
    Разбивает строку на токены, в качестве делителя используется пробел. Все пробелы исключаются из выхода.
    Вход:
        Line - строка.
    Выход:
        Массив токенов, без пробелов.     
    */
    static std::vector<std::string> _parse_the_line(std::string Line);

    /* Добавляет спутник в массив спутников, если его там нет.
    Вход:
        Settings - параметры загрузки
        Name - название файла
        Satellites - выходной массив спутников
    Выход:
        ID спутника.
    */
    static size_t _add_satellite(const settings & Settings, const std::string & Name, std::vector<satellite> & Satellites);

    /* Добавляет станцию и спутникв в массивы, если их там нет.
    Вход:
        Settings - параметры загрузки
        Name - считанный из файла токен
        Stations - выходной массив станций
    Выход:
        ID станции.
    */
    static size_t _add_station(const settings & Settings, const std::string & Name,  std::vector<station> & Stations);

    /* Добавляет событие пролета над россией.
    Вход:
        SatelliteID - ID спутника
        Tokens - считанный массив токенов из файла
        Events - выходной вектор событий
    */
    static void _add_russia_events(size_t SatelliteID, const std::vector<std::string> & Tokens, std::vector<event> & Events);

    /* Добавляет события зоны видимости спутника.
    Вход:
        StationID - ID станции
        SatelliteID - ID спутника
        Tokens - считанный массив токенов из файла
        Events - выходной вектор событий
    */
    static void _add_visibility_events(size_t StationID, size_t SatelliteID, const std::vector<std::string> & Tokens, std::vector<event> & Events);

    /* 
    Вход:
        Month - сокращенное название месяца (Jan, Dec, etc.)
    Выход:
        Номер месяца в году
    */
    static std::string _get_month(const std::string & Month);

};

}