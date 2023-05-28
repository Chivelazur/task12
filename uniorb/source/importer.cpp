#include "../include/importer.hpp"
#include "../include/date.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

namespace uniorb {

bool importer::import_data(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites, std::vector<station> & Stations) {
    // Чистим входные данные.

    // Считываем данные.
    bool Res = _import_russia_flights(Settings, Events, Satellites) && _import_station_visibility(Settings, Events, Satellites, Stations);

    // Сортируем вектор событий.
    std::sort(Events.begin(), Events.end(), event::compare_events);
    return Res;
}

bool importer::_import_russia_flights(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites) {
    std::cout << "importer::_import_russia_flights. Start loading files of on-Russia flights." << std::endl;
    clock_t timer = std::clock();

    // Массив с файлами
    std::vector<std::string> FilePaths = std::vector<std::string> ();

    // Получаем список всех файлов.
    try {
        for (const auto & Entry : std::filesystem::directory_iterator(Settings.russia_dir_path)) {
            auto Path = Entry.path().string();
            if (Path.find("readme") == std::string::npos) {
                FilePaths.push_back(Entry.path().string());
            }
        }
    }
    catch (std::exception) {
        std::cout << "importer::_import_russia_flights. Failed to get list of files." << std::endl;
        return false;
    }

    // Считываем по-файлово
    for (const auto & FilePath : FilePaths) {
        std::ifstream file(FilePath);
        if (!file.good()) {
            std::cout << "importer::_import_russia_flights. Failed to open the file " + FilePath << std::endl;
            return false;
        }

        // Считываем
        std::string line;
        auto Tokens = std::vector<std::string>();

        // Пропускаем первые 5 строк
        for (int i = 0; i < 5; ++i) std::getline(file, line);

        // Далее построчно
        while (std::getline(file, line)){
            Tokens = _parse_the_line(line);
            // Если нашли строку вида ***-To-***
            if (Tokens.size() != 0 && Tokens[0].find("-To-") != std::string::npos) {
                // Создаем спутник по этом токену (без дублей)
                // Пример токена: Russia-To-KinoSat_110103
                // Убираем первые 10 символов
                auto Name = Tokens[0].substr(10, Tokens[0].size());
                size_t SatelliteID = _add_satellite(Settings, Name, Satellites);
                // Пропускаем три строки
                for (int i = 0; i < 3; ++i) std::getline(file, line);

                // И далее считываем события до пустой строки.
                while (std::getline(file, line)){
                    Tokens = _parse_the_line(line);
                    if (Tokens.size() == 0) {
                        break;
                    }
                    _add_russia_events(SatelliteID, Tokens, Events);
                }
            }
        }
        file.close();
    }

    std::cout << "importer::_import_russia_flights. Successfully loaded "<< FilePaths.size() << " files in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

bool importer::_import_station_visibility(const settings & Settings, std::vector<event> & Events, std::vector<satellite> & Satellites, std::vector<station> & Stations) {
    std::cout << "importer::_import_station_visibility. Start loading visibility files." << std::endl;
    clock_t timer = std::clock();
    
    // Массив с файлами
    std::vector<std::string> FilePaths = std::vector<std::string> ();

    // Получаем список всех файлов.
    try {
        for (const auto & Entry : std::filesystem::directory_iterator(Settings.visibility_dir_path)) {
            auto Path = Entry.path().string();
            if (Path.find("readme") == std::string::npos) {
                FilePaths.push_back(Entry.path().string());
            }
        }
    }
    catch (std::exception) {
        std::cout << "importer::_import_station_visibility. Failed to get list of files." << std::endl;
        return false;
    }

    // Считываем по-файлово
    for (const auto & FilePath : FilePaths) {
        std::ifstream file(FilePath);
        if (!file.good()) {
            std::cout << "importer::_import_station_visibility. Failed to open the file " + FilePath << std::endl;
            return false;
        }

        // Считываем
        std::string line;
        auto Tokens = std::vector<std::string>();

        // Пропускаем первые 5 строк
        for (int i = 0; i < 5; ++i) std::getline(file, line);

        while (std::getline(file, line)){
            Tokens = _parse_the_line(line);
            // Если нашли строку вида ***-To-***
            if (Tokens.size() != 0 && Tokens[0].find("-To-") != std::string::npos) {
                // Создаем спутник и станцию по этом токену (без дублей)
                // Пример токена: Magadan2-To-KinoSat_110101
                auto Delimeter = Tokens[0].find("-To-");
                auto StationName = Tokens[0].substr(0, Delimeter);
                auto SatelliteName = Tokens[0].substr(Delimeter + 4, Tokens[0].size());
                auto StationID = _add_station(Settings, StationName, Stations);
                auto SatelliteID = _add_satellite(Settings, SatelliteName, Satellites);

                // Пропускаем три строки
                for (int i = 0; i < 3; i++) std::getline(file, line);

                // и далее считываем события до пустой строки.
                while (std::getline(file, line)){
                    Tokens = _parse_the_line(line);
                    if (Tokens.size() == 0) {
                        break;
                    }
                    _add_visibility_events(StationID, SatelliteID, Tokens, Events);
                }
            }
        }
        file.close();
    }

    std::cout << "importer::_import_station_visibility. Successfully loaded "<< FilePaths.size() << " files in " << (std::clock() - timer) / (double) CLOCKS_PER_SEC << " s." << std::endl;
    return true;
}

std::vector<std::string> importer::_parse_the_line(std::string Line) {
    auto Tokens = std::vector<std::string>();
    std::istringstream iss(Line);
    std::string Token;
    while (getline(iss >> std::ws, Token, ' ') ) {
        Tokens.push_back(Token);
    }
    return Tokens;
}

size_t importer::_add_satellite(const settings & Settings, const std::string & Name, std::vector<satellite> & Satellites) {
    // Проверяем, есть ли этот спутник
    for (const auto & Satellite : Satellites) {
        if (Satellite.name == Name) {
            return Satellite.id;
        }
    }
    
    // Определяем тип спутника
    // Аппараты Kinosat_110101 .. Kinosat_110510 - спутники серии Киноспутник (высокодетальное ДЗЗ)
    // Аппараты Kinosat_110601 .. Kinosat_112010 - спутники серии Зоркий (ДЗЗ среднего разрешения)
    int Number = std::stoi(Name.substr(8, 6));

    // Добавляем спутник Киноспутник
    if (Number < 110601) {
        auto Satellite = satellite(Name, Settings.kinosat_parameters);
        Satellites.push_back(Satellite);
        return Satellite.id;
    }

    // Добавляем спутник Зоркий
    if (Number >= 110601) {
        auto Satellite = satellite(Name, Settings.zorkiy_parameters);
        Satellites.push_back(Satellite);
        return Satellite.id;
    }

    return 0;
}

size_t importer::_add_station(const settings & Settings, const std::string & Name, std::vector<station> & Stations) {
    // Проверяем, есть ли эта станция
    for (const auto & Station : Stations) {
        if (Station.name == Name) {
            return Station.id;
        }
    }
    // Добавляем станцию
    auto Station = station(Name);
    Stations.push_back(Station);
    return Station.id;
}

void importer::_add_russia_events(size_t SatelliteID, const std::vector<std::string> & Tokens, std::vector<event> & Events) {
    // 228    13 Jun 2027 21:37:56.001    13 Jun 2027 21:38:08.859            12.858
    // YYYY-MM-DDTHH:MM:SS.SSS
    date EntranceEpoch = date(Tokens[3] + "-" + _get_month(Tokens[2]) + "-" + Tokens[1] + "T" + Tokens[4]);
    date ExitEpoch = date(Tokens[7] + "-" + _get_month(Tokens[6]) + "-" + Tokens[5] + "T" + Tokens[8]);;

    // Добавляем события.
    auto RussiaEntrance = event();
    RussiaEntrance.epoch = EntranceEpoch.get_mjd();
    RussiaEntrance.satellite_id = SatelliteID;
    RussiaEntrance.station_id = 0;
    RussiaEntrance.type = russia_entrance;

    auto RussiaExit = event();
    RussiaExit.epoch = ExitEpoch.get_mjd();
    RussiaExit.satellite_id = SatelliteID;
    RussiaExit.station_id = 0;
    RussiaExit.type = russia_exit;

    Events.push_back(RussiaEntrance);
    Events.push_back(RussiaExit);
}

void importer::_add_visibility_events(size_t StationID, size_t SatelliteID, const std::vector<std::string> & Tokens, std::vector<event> & Events) {
    // 1     1 Jun 2027 00:00:01.000     1 Jun 2027 00:04:10.816           249.816
    // YYYY-MM-DDTHH:MM:SS.SSS
    date EntranceEpoch = date(Tokens[3] + "-" + _get_month(Tokens[2]) + "-" + Tokens[1] + "T" + Tokens[4]);
    date ExitEpoch = date(Tokens[7] + "-" + _get_month(Tokens[6]) + "-" + Tokens[5] + "T" + Tokens[8]);;

    // Добавляем события.
    auto StationEntrance = event();
    StationEntrance.epoch = EntranceEpoch.get_mjd();
    StationEntrance.satellite_id = SatelliteID;
    StationEntrance.station_id = StationID;
    StationEntrance.type = station_entrance;

    auto StationExit = event();
    StationExit.epoch = ExitEpoch.get_mjd();
    StationExit.satellite_id = SatelliteID;
    StationExit.station_id = StationID;
    StationExit.type = station_exit;

    Events.push_back(StationEntrance);
    Events.push_back(StationExit);
}

std::string importer::_get_month(const std::string & Month) {
    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
	static auto Months = std::unordered_map<std::string, std::string> ();
    Months.insert_or_assign("Jan", "01");
    Months.insert_or_assign("Feb", "02");
    Months.insert_or_assign("Mar", "03");
    Months.insert_or_assign("Apr", "04");
    Months.insert_or_assign("May", "05");
    Months.insert_or_assign("Jun", "06");
    Months.insert_or_assign("Jul", "07");
    Months.insert_or_assign("Aug", "08");
    Months.insert_or_assign("Sep", "09");
    Months.insert_or_assign("Oct", "10");
    Months.insert_or_assign("Nov", "11");
    Months.insert_or_assign("Dec", "12");
    return Months.at(Month);
}

}